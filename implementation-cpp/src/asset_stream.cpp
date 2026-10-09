// no-copy asset streaming. Fetch loads every bundle from {persistentDataPath}/Resources/
// {ToAssetPath(location)} and throws if it is missing -- the load path never downloads on
// demand -- so without this a full on-disk mirror would be required. we hook ToAssetPath (the
// resolver Fetch calls right before File.Exists), stream any missing file from the in-process
// server into place so the following LoadFromFileAsync finds it, and pass the return value
// through. the cache is wiped per launch and size-capped; CRI is never evicted (CriWare streams
// it by path). ToAssetPath is static: Il2CppString* ToAssetPath(IResourceLocation*, void* mi).

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>

#include "asset_stream.h"
#include "hook_slot.h"

#ifdef __ANDROID__
#include <android/log.h>
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "[UTSK GD]", __VA_ARGS__)
#define PLATFORM "Android"
#elif defined(__APPLE__)
#include <os/log.h>
#define LOG(fmt, ...) do { char _b[1024]; snprintf(_b, sizeof(_b), fmt, ##__VA_ARGS__); os_log(OS_LOG_DEFAULT, "[UTSK GD] %{public}s", _b); } while(0)
#define PLATFORM "iOS"
#else
#define LOG(...)
#define PLATFORM "Android"
#endif

static constexpr int SERVER_PORT = 39046;
static constexpr size_t CACHE_CAP_BYTES = 4ull * 1024 * 1024 * 1024;  // evict oldest non-CRI past this

struct Il2CppString {
    void* klass;
    void* monitor;
    int32_t length;
    uint16_t chars[1];
};

typedef Il2CppString* (*to_asset_path_t)(void* location, void* method);
typedef Il2CppString* (*get_pdp_t)(void* method);

static to_asset_path_t orig_toassetpath = nullptr;
static get_pdp_t        g_get_pdp = nullptr;

static std::string g_base_dir;                       // {persistentDataPath}/Resources, resolved lazily
static std::once_flag g_init_once;

static std::mutex g_cache_mtx;
static std::unordered_set<std::string> g_miss;       // paths the server lacks: don't retry
struct CacheEntry { std::string path; size_t size; };
static std::vector<CacheEntry> g_lru;                // non-CRI streamed files, insertion order
static size_t g_lru_bytes = 0;

static std::string u16_to_utf8(const Il2CppString* s) {
    std::string out;
    if (!s) return out;
    for (int i = 0; i < s->length; i++) {
        uint32_t c = s->chars[i];
        if (c < 0x80) out.push_back((char)c);
        else if (c < 0x800) {
            out.push_back((char)(0xC0 | (c >> 6)));
            out.push_back((char)(0x80 | (c & 0x3F)));
        } else {
            out.push_back((char)(0xE0 | (c >> 12)));
            out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
            out.push_back((char)(0x80 | (c & 0x3F)));
        }
    }
    return out;
}

static bool http_get(const std::string& path, std::string& out) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;
    struct sockaddr_in sa = {};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVER_PORT);
    sa.sin_addr.s_addr = htonl(0x7F000001);
    if (connect(fd, (struct sockaddr*)&sa, sizeof(sa)) != 0) { close(fd); return false; }
    std::string req = "GET " + path + " HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    if (send(fd, req.data(), req.size(), 0) != (ssize_t)req.size()) { close(fd); return false; }
    std::string resp;
    char buf[65536];
    ssize_t n;
    while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) resp.append(buf, n);
    close(fd);
    size_t hdr_end = resp.find("\r\n\r\n");
    if (hdr_end == std::string::npos) return false;
    if (resp.compare(0, 12, "HTTP/1.1 200") != 0 && resp.compare(0, 12, "HTTP/1.0 200") != 0)
        return false;
    out = resp.substr(hdr_end + 4);
    return !out.empty();
}

static void rmrf_contents(const std::string& dir) {
    DIR* d = opendir(dir.c_str());
    if (!d) return;
    struct dirent* e;
    while ((e = readdir(d)) != nullptr) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        std::string p = dir + "/" + e->d_name;
        struct stat st;
        if (lstat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
            rmrf_contents(p);
            rmdir(p.c_str());
        } else {
            unlink(p.c_str());
        }
    }
    closedir(d);
}

static void make_parent_dirs(const std::string& path) {
    size_t slash = path.find_last_of('/');
    if (slash == std::string::npos) return;
    std::string dir = path.substr(0, slash);
    for (size_t i = 1; i < dir.size(); i++) {
        if (dir[i] == '/') {
            std::string sub = dir.substr(0, i);
            mkdir(sub.c_str(), 0755);
        }
    }
    mkdir(dir.c_str(), 0755);
}

// once, before any stream (call_once blocks the others): resolve the Resources root and wipe
// last launch's cache so only this session's working set stays on disk
static void init_once() {
    if (!g_get_pdp) return;
    std::string p = u16_to_utf8(g_get_pdp(nullptr));
    if (p.empty()) return;
    g_base_dir = p + "/Resources";
    rmrf_contents(g_base_dir);
    LOG("[asset_stream] base dir = %s (wiped fresh)", g_base_dir.c_str());
}

// evict oldest streamed file once over the cap -- safe since an open AssetBundle keeps the
// inode (POSIX) and a re-request re-streams
static void cache_record(const std::string& path, size_t size) {
    std::lock_guard<std::mutex> lk(g_cache_mtx);
    g_lru.push_back({path, size});
    g_lru_bytes += size;
    while (g_lru_bytes > CACHE_CAP_BYTES && g_lru.size() > 1) {
        CacheEntry old = g_lru.front();
        g_lru.erase(g_lru.begin());
        g_lru_bytes -= old.size;
        unlink(old.path.c_str());
    }
}

// stream rel into localPath from the local server if absent; kind is unknown here so try each
// (cri-assets stays off the eviction cap -- CriWare reads it by path during playback)
static void ensure_file(const std::string& rel, const std::string& localPath) {
    { std::lock_guard<std::mutex> lk(g_cache_mtx); if (g_miss.count(localPath)) return; }
    struct stat st;
    if (stat(localPath.c_str(), &st) == 0 && st.st_size > 0) return;

    static const char* KINDS[] = {"2d-assets", "3d-assets", "cri-assets"};
    std::string body;
    const char* hit_kind = nullptr;
    for (const char* kind : KINDS) {
        std::string url = std::string("/production/") + kind + "/" + PLATFORM + "/0/" + rel;
        if (http_get(url, body)) { hit_kind = kind; break; }
    }
    if (!hit_kind) {
        std::lock_guard<std::mutex> lk(g_cache_mtx);
        g_miss.insert(localPath);
        return;
    }
    make_parent_dirs(localPath);
    FILE* f = fopen(localPath.c_str(), "wb");
    if (!f) { LOG("[asset_stream] open failed %s", localPath.c_str()); return; }
    fwrite(body.data(), 1, body.size(), f);
    fclose(f);
    if (strcmp(hit_kind, "cri-assets") != 0) cache_record(localPath, body.size());
    LOG("[asset_stream] streamed %s/%s (%zuB)", hit_kind, rel.c_str(), body.size());
}

static Il2CppString* hook_toassetpath(void* location, void* method) {
    Il2CppString* rel = orig_toassetpath ? orig_toassetpath(location, method) : nullptr;
    std::call_once(g_init_once, init_once);
    if (rel && !g_base_dir.empty()) {
        std::string r = u16_to_utf8(rel);
        if (!r.empty()) ensure_file(r, g_base_dir + "/" + r);
    }
    return rel;
}

void asset_stream_install(uintptr_t module_base, uintptr_t orig_rva,
                          uintptr_t slot_rva, uintptr_t get_pdp_rva) {
    orig_toassetpath = (to_asset_path_t)(module_base + orig_rva);
    g_get_pdp = get_pdp_rva ? (get_pdp_t)(module_base + get_pdp_rva) : nullptr;
    write_hook_slot(module_base + slot_rva, (uintptr_t)hook_toassetpath);
    LOG("[asset_stream] ToAssetPath hooked (orig @ 0x%lx, slot @ 0x%lx)",
        (unsigned long)(module_base + orig_rva), (unsigned long)(module_base + slot_rva));
}
