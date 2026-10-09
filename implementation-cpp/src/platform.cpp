#include "platform.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <stdlib.h>
#include <map>
#include <mutex>
#include <vector>
#include <string.h>
#include <errno.h>
#include "miniz.h"

#ifdef __ANDROID__
#include <android/log.h>
#include <unistd.h>
#include <dirent.h>
#include <jni.h>
#include <dlfcn.h>

#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "[UTSK GD]", fmt, ##__VA_ARGS__)
#elif defined(__APPLE__)
#include <dlfcn.h>
#include <os/log.h>
#define LOG(fmt, ...) do { char _logbuf[1024]; snprintf(_logbuf, sizeof(_logbuf), fmt, ##__VA_ARGS__); os_log(OS_LOG_DEFAULT, "[UTSK GD] %{public}s", _logbuf); } while(0)
#else
#include <cstdio>
#define LOG(fmt, ...) printf("[UTSK GD] " fmt "\n", ##__VA_ARGS__)
#endif

static void mkdirs(const std::string& path) {
    std::string cur;
    for (size_t i = 0; i < path.size(); i++) {
        cur += path[i];
        if (path[i] == '/') mkdir(cur.c_str(), 0755);
    }
    mkdir(path.c_str(), 0755);
}

bool platform::write_file(const std::string& path, const std::string& data) {
    auto slash = path.rfind('/');
    if (slash != std::string::npos) mkdirs(path.substr(0, slash));
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f.is_open()) return false;
    f.write(data.data(), (std::streamsize)data.size());
    return f.good();
}

// ============================================================================
// ZIP — shared across all platforms (uses miniz)
// ============================================================================

// several portion zips are mounted together into one combined index (base_<platform>.zip +
// extra_<platform>.zip the user drops on the device), so an asset is found in whichever holds it
struct ZipArchive {
    mz_zip_archive z{};
    FILE* cfile = nullptr;
    std::string path;
};
static std::vector<ZipArchive*> g_zips;
static std::map<std::string, std::pair<size_t, mz_uint>> g_zip_index;
static std::mutex g_zip_mutex;

// open one zip and merge its files into the combined index (first zip wins a duplicate).
// strip_prefix is removed from each entry; key_prefix is then prepended. portion entries are
// "<kind>/<platform>/<rel>", indexed under "_data/assets/<entry>" so the server's
// read_file("_data/assets/...") asset lookups resolve against whichever portion holds them.
static bool zip_open(const std::string& zip_path, const std::string& strip_prefix,
                     const std::string& key_prefix = "", int fd = -1) {
    ZipArchive* a = new ZipArchive();
    a->path = zip_path;
    bool opened = false;
    if (fd >= 0) {
        a->cfile = fdopen(fd, "rb");
        if (a->cfile) {
            fseek(a->cfile, 0, SEEK_END);
            mz_uint64 size = (mz_uint64)ftell(a->cfile);
            fseek(a->cfile, 0, SEEK_SET);
            opened = mz_zip_reader_init_cfile(&a->z, a->cfile, size, 0);
            if (!opened) { fclose(a->cfile); a->cfile = nullptr; }
        } else {
            close(fd);
        }
    } else {
        opened = mz_zip_reader_init_file(&a->z, zip_path.c_str(), 0);
    }
    if (!opened) {
        LOG("WARNING: Failed to open zip: %s (error %d)", zip_path.c_str(), (int)a->z.m_last_error);
        delete a;
        return false;
    }

    size_t ai = g_zips.size();
    mz_uint n = mz_zip_reader_get_num_files(&a->z);
    size_t prefix_len = strip_prefix.size();
    int indexed = 0;
    for (mz_uint i = 0; i < n; i++) {
        char name[512];
        mz_uint name_len = mz_zip_reader_get_filename(&a->z, i, name, sizeof(name));
        if (name_len == 0) continue;
        if (mz_zip_reader_is_file_a_directory(&a->z, i)) continue;
        std::string sname(name);
        if (sname.size() <= prefix_len || sname.compare(0, prefix_len, strip_prefix) != 0) continue;
        std::string rel = key_prefix + sname.substr(prefix_len);
        if (g_zip_index.find(rel) == g_zip_index.end()) {
            g_zip_index[rel] = {ai, i};
            indexed++;
        }
    }

    if (indexed > 0) {
        g_zips.push_back(a);
        LOG("Zip indexed: %d entries from %s", indexed, zip_path.c_str());
        return true;
    }
    LOG("WARNING: Zip indexed 0 entries from %s", zip_path.c_str());
    mz_zip_reader_end(&a->z);
    if (a->cfile) fclose(a->cfile);
    delete a;
    return false;
}

// mount base_<platform>.zip + extra_<platform>.zip from `dirs` (first dir holding each wins).
// fd_opener (optional) reopens candidates fopen can't read directly (Android scoped storage).
static void open_portion_zips(const std::vector<std::string>& dirs, const std::string& platform_str,
                              int (*fd_opener)(const std::string&) = nullptr) {
    static const char* GROUPS[] = {"base", "extra"};
    for (const char* group : GROUPS) {
        std::string fname = std::string(group) + "_" + platform_str + ".zip";
        bool done = false;
        std::vector<std::string> denied;
        for (const auto& dir : dirs) {
            std::string cand = dir + "/" + fname;
            FILE* t = fopen(cand.c_str(), "rb");
            if (t) { fclose(t); if (zip_open(cand, "", "_data/assets/")) { done = true; break; } }
            else if (errno == EACCES) denied.push_back(cand);
        }
        if (!done && fd_opener)
            for (const auto& cand : denied) {
                int fd = fd_opener(cand);
                if (fd >= 0 && zip_open(cand, "", "_data/assets/", fd)) break;
            }
    }
    LOG("Mounted %zu portion zip(s), %zu entries", g_zips.size(), g_zip_index.size());
}

static std::string zip_read_file(const std::string& path) {
    auto it = g_zip_index.find(path);
    if (it == g_zip_index.end()) return "";

    std::lock_guard<std::mutex> lock(g_zip_mutex);
    ZipArchive* a = g_zips[it->second.first];
    size_t size = 0;
    void* data = mz_zip_reader_extract_to_heap(&a->z, it->second.second, &size, 0);
    if (!data) {
        LOG("WARNING: zip extraction failed for %s (error %d)", path.c_str(), (int)a->z.m_last_error);
        return "";
    }
    std::string result((const char*)data, size);
    mz_free(data);
    return result;
}

static bool zip_has_file(const std::string& path) {
    return g_zip_index.count(path) > 0;
}

std::vector<std::string> platform::verify_assets(const std::string& os, const std::vector<std::string>& bundle_names) {
    std::vector<std::string> missing;
    for (auto& name : bundle_names) {
        std::string path = "assets/" + os + "/" + name;
        if (!zip_has_file(path))
            missing.push_back(name);
    }
    return missing;
}

// ============================================================================
// ANDROID
// ============================================================================

#ifdef __ANDROID__

static std::string g_data_dir;
static std::string g_ext_dir;
static std::vector<std::string> g_search_dirs;

static std::string get_package_name() {
    std::ifstream f("/proc/self/cmdline");
    std::string name;
    std::getline(f, name, '\0');
    return name;
}

static std::string parent_dir(const std::string& path) {
    auto pos = path.rfind('/');
    return (pos != std::string::npos) ? path.substr(0, pos) : path;
}

static void add_unique_dir(std::vector<std::string>& dirs, const std::string& path) {
    for (auto& d : dirs)
        if (d == path) return;
    mkdirs(path);
    dirs.push_back(path);
}

// ===== JNI =====

typedef jint (*JNI_GetCreatedJavaVMs_t)(JavaVM**, jsize, jsize*);

static JavaVM* get_jvm() {
    JavaVM* vm = nullptr;
    jsize count = 0;
    JNI_GetCreatedJavaVMs_t fn = (JNI_GetCreatedJavaVMs_t)dlsym(RTLD_DEFAULT, "JNI_GetCreatedJavaVMs");
    if (!fn) {
        const char* libs[] = { "libnativehelper.so", "libart.so", "libandroid_runtime.so", nullptr };
        for (int i = 0; libs[i]; i++) {
            void* handle = dlopen(libs[i], RTLD_NOLOAD);
            if (!handle) handle = dlopen(libs[i], RTLD_NOW);
            if (!handle) continue;
            fn = (JNI_GetCreatedJavaVMs_t)dlsym(handle, "JNI_GetCreatedJavaVMs");
            if (fn) break;
        }
    }
    if (fn && fn(&vm, 1, &count) == JNI_OK && count > 0) return vm;
    return nullptr;
}

static std::string jstring_to_string(JNIEnv* env, jstring js) {
    if (!js) return "";
    const char* utf = env->GetStringUTFChars(js, nullptr);
    if (!utf) return "";
    std::string result(utf);
    env->ReleaseStringUTFChars(js, utf);
    return result;
}

static std::string file_get_path(JNIEnv* env, jobject file) {
    if (!file) return "";
    jclass cls = env->GetObjectClass(file);
    jmethodID mid = env->GetMethodID(cls, "getAbsolutePath", "()Ljava/lang/String;");
    auto path = (jstring)env->CallObjectMethod(file, mid);
    env->DeleteLocalRef(cls);
    return jstring_to_string(env, path);
}

static jobject get_app_context(JNIEnv* env) {
    jclass at_cls = env->FindClass("android/app/ActivityThread");
    if (env->ExceptionCheck()) { env->ExceptionClear(); return nullptr; }
    jmethodID ca = env->GetStaticMethodID(at_cls, "currentApplication", "()Landroid/app/Application;");
    jobject ctx = env->CallStaticObjectMethod(at_cls, ca);
    env->DeleteLocalRef(at_cls);
    return ctx;
}

static void jni_add_context_dirs(std::vector<std::string>& dirs) {
    JavaVM* vm = get_jvm();
    if (!vm) return;

    JNIEnv* env = nullptr;
    bool attached = false;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return;
        attached = true;
    }

    jobject ctx = get_app_context(env);
    if (!ctx) {
        if (attached) vm->DetachCurrentThread();
        return;
    }
    jclass ctx_cls = env->GetObjectClass(ctx);

    auto add_file_dir = [&](const char* method, const char* sig) {
        jmethodID mid = env->GetMethodID(ctx_cls, method, sig);
        jobject f = env->CallObjectMethod(ctx, mid);
        std::string p = file_get_path(env, f);
        if (!p.empty()) {
            add_unique_dir(dirs, p + "/gridlessdreams");
            add_unique_dir(dirs, p);
        }
        if (f) env->DeleteLocalRef(f);
    };

    auto add_file_dir_arg = [&](const char* method, const char* sig) {
        jmethodID mid = env->GetMethodID(ctx_cls, method, sig);
        jobject f = env->CallObjectMethod(ctx, mid, (jstring)nullptr);
        std::string p = file_get_path(env, f);
        if (!p.empty()) {
            add_unique_dir(dirs, p + "/gridlessdreams");
            add_unique_dir(dirs, p);
        }
        if (f) env->DeleteLocalRef(f);
    };

    auto add_file_dirs_array = [&](const char* method, const char* sig) {
        jmethodID mid = env->GetMethodID(ctx_cls, method, sig);
        auto arr = (jobjectArray)env->CallObjectMethod(ctx, mid, (jstring)nullptr);
        if (!arr) return;
        jsize len = env->GetArrayLength(arr);
        for (jsize i = 0; i < len; i++) {
            jobject f = env->GetObjectArrayElement(arr, i);
            std::string p = file_get_path(env, f);
            if (!p.empty()) {
                add_unique_dir(dirs, p + "/gridlessdreams");
                add_unique_dir(dirs, p);
            }
            if (f) env->DeleteLocalRef(f);
        }
        env->DeleteLocalRef(arr);
    };

    add_file_dir("getFilesDir", "()Ljava/io/File;");
    add_file_dir("getCacheDir", "()Ljava/io/File;");
    add_file_dir_arg("getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");
    add_file_dirs_array("getExternalFilesDirs", "(Ljava/lang/String;)[Ljava/io/File;");
    add_file_dir("getObbDir", "()Ljava/io/File;");

    env->DeleteLocalRef(ctx_cls);
    env->DeleteLocalRef(ctx);
    if (attached) vm->DetachCurrentThread();
}

static int jni_open_file(const std::string& path) {
    JavaVM* vm = get_jvm();
    if (!vm) return -1;

    JNIEnv* env = nullptr;
    bool attached = false;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return -1;
        attached = true;
    }

    int fd = -1;
    jclass fis_cls = env->FindClass("java/io/FileInputStream");
    if (!env->ExceptionCheck() && fis_cls) {
        jmethodID ctor = env->GetMethodID(fis_cls, "<init>", "(Ljava/lang/String;)V");
        jstring jpath = env->NewStringUTF(path.c_str());
        jobject fis = env->NewObject(fis_cls, ctor, jpath);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        } else if (fis) {
            jmethodID getFD = env->GetMethodID(fis_cls, "getFD", "()Ljava/io/FileDescriptor;");
            jobject fdObj = env->CallObjectMethod(fis, getFD);
            if (fdObj) {
                jclass fd_cls = env->FindClass("java/io/FileDescriptor");
                jfieldID fd_field = env->GetFieldID(fd_cls, "descriptor", "I");
                fd = dup(env->GetIntField(fdObj, fd_field));
                env->DeleteLocalRef(fd_cls);
                env->DeleteLocalRef(fdObj);
            }
            jmethodID closeMid = env->GetMethodID(fis_cls, "close", "()V");
            env->CallVoidMethod(fis, closeMid);
            if (env->ExceptionCheck()) env->ExceptionClear();
            env->DeleteLocalRef(fis);
        }
        env->DeleteLocalRef(jpath);
        env->DeleteLocalRef(fis_cls);
    } else {
        if (env->ExceptionCheck()) env->ExceptionClear();
    }

    if (attached) vm->DetachCurrentThread();
    return fd;
}

static std::string jni_read_file(const std::string& path) {
    JavaVM* vm = get_jvm();
    if (!vm) return "";

    JNIEnv* env = nullptr;
    bool attached = false;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return "";
        attached = true;
    }

    std::string result;
    jclass fis_cls = env->FindClass("java/io/FileInputStream");
    if (!env->ExceptionCheck() && fis_cls) {
        jmethodID ctor = env->GetMethodID(fis_cls, "<init>", "(Ljava/lang/String;)V");
        jstring jpath = env->NewStringUTF(path.c_str());
        jobject fis = env->NewObject(fis_cls, ctor, jpath);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        } else if (fis) {
            jmethodID readMid = env->GetMethodID(fis_cls, "read", "([B)I");
            jmethodID closeMid = env->GetMethodID(fis_cls, "close", "()V");
            jbyteArray buf = env->NewByteArray(8192);
            while (true) {
                jint n = env->CallIntMethod(fis, readMid, buf);
                if (env->ExceptionCheck()) { env->ExceptionClear(); break; }
                if (n <= 0) break;
                jbyte* data = env->GetByteArrayElements(buf, nullptr);
                result.append((const char*)data, n);
                env->ReleaseByteArrayElements(buf, data, JNI_ABORT);
            }
            env->CallVoidMethod(fis, closeMid);
            if (env->ExceptionCheck()) env->ExceptionClear();
            env->DeleteLocalRef(buf);
            env->DeleteLocalRef(fis);
        }
        env->DeleteLocalRef(jpath);
        env->DeleteLocalRef(fis_cls);
    } else {
        if (env->ExceptionCheck()) env->ExceptionClear();
    }

    if (attached) vm->DetachCurrentThread();
    return result;
}

// ===== PATHS =====

static std::string find_files_dir(const std::string& pkg) {
    const char* tmpdir = getenv("TMPDIR");
    if (tmpdir) {
        std::string t(tmpdir);
        auto pos = t.rfind("/cache");
        if (pos != std::string::npos)
            return t.substr(0, pos) + "/files";
    }
    return "/data/data/" + pkg + "/files";
}

static void build_hardcoded_dirs(std::vector<std::string>& dirs, const std::string& pkg) {
    add_unique_dir(dirs, "/data/data/" + pkg + "/files/gridlessdreams");
    add_unique_dir(dirs, "/data/data/" + pkg + "/files");
    add_unique_dir(dirs, "/data/user/0/" + pkg + "/files/gridlessdreams");
    add_unique_dir(dirs, "/data/user/0/" + pkg + "/files");
    add_unique_dir(dirs, "/sdcard/Android/data/" + pkg + "/files/gridlessdreams");
    add_unique_dir(dirs, "/sdcard/Android/data/" + pkg + "/files");
    add_unique_dir(dirs, "/storage/emulated/0/Android/data/" + pkg + "/files/gridlessdreams");
    add_unique_dir(dirs, "/storage/emulated/0/Android/data/" + pkg + "/files");
    add_unique_dir(dirs, "/storage/self/primary/Android/data/" + pkg + "/files/gridlessdreams");
    add_unique_dir(dirs, "/storage/self/primary/Android/data/" + pkg + "/files");
    // OBB dirs — always readable by the app, no permissions needed
    add_unique_dir(dirs, "/storage/emulated/0/Android/obb/" + pkg);
    add_unique_dir(dirs, "/sdcard/Android/obb/" + pkg);
    add_unique_dir(dirs, "/storage/self/primary/Android/obb/" + pkg);
}

static std::string find_apk_path(const std::string& pkg) {
    std::ifstream f("/proc/self/maps");
    std::string line;
    while (std::getline(f, line)) {
        if (line.find(".apk") == std::string::npos) continue;
        if (line.find(pkg) == std::string::npos) continue;
        auto slash = line.find('/');
        if (slash == std::string::npos) continue;
        std::string path = line.substr(slash);
        while (!path.empty() && (path.back() <= ' ')) path.pop_back();
        if (access(path.c_str(), R_OK) == 0) return path;
    }
    return "";
}

static std::string read_marker(const std::string& path) {
    std::ifstream f(path);
    std::string s;
    std::getline(f, s, '\0');  // whole file (a path has no embedded NUL)
    return s;
}

static void extract_apk_assets(const std::string& apk_path, const std::string& dest_dir) {
    mz_zip_archive apk = {};
    if (!mz_zip_reader_init_file(&apk, apk_path.c_str(), 0)) {
        LOG("Can't open APK: %s", apk_path.c_str());
        return;
    }

    // re-extract whenever the apk changes. the install-hash in apk_path changes on every
    // (re)install and app update, so bundled data -- above all hooks.json, which is
    // build-specific (trampoline/slot RVAs) -- never goes stale. a normal relaunch of the
    // same install matches the marker and skips (fast); otherwise every file is overwritten.
    std::string marker = dest_dir + "/.apk_src";
    bool force = (read_marker(marker) != apk_path);

    const char* prefix = "assets/gridlessdreams/";
    size_t prefix_len = strlen(prefix);
    mz_uint n = mz_zip_reader_get_num_files(&apk);
    int extracted = 0;

    for (mz_uint i = 0; i < n; i++) {
        char name[512];
        mz_zip_reader_get_filename(&apk, i, name, sizeof(name));
        if (mz_zip_reader_is_file_a_directory(&apk, i)) continue;

        std::string sname(name);
        if (sname.size() <= prefix_len || sname.compare(0, prefix_len, prefix) != 0) continue;
        std::string rel = sname.substr(prefix_len);
        if (rel.empty()) continue;

        std::string dest = dest_dir + "/" + rel;
        // hooks.json is build-specific (trampoline/slot RVAs); always refresh it so a
        // reinstall never runs against a stale copy even if the apk-change marker misfires
        bool always = (rel == "hooks.json");
        if (!always && !force && access(dest.c_str(), F_OK) == 0) continue;

        auto last_slash = dest.rfind('/');
        if (last_slash != std::string::npos) mkdirs(dest.substr(0, last_slash));

        if (mz_zip_reader_extract_to_file(&apk, i, dest.c_str(), 0)) {
            extracted++;
            LOG("Extracted: %s", rel.c_str());
        }
    }

    mz_zip_reader_end(&apk);
    if (extracted > 0) LOG("Extracted %d files from APK", extracted);
    std::ofstream(marker) << apk_path;  // record which apk these files came from
}

void platform::init_android() {
    std::string pkg = get_package_name();
    LOG("Package: %s", pkg.c_str());

    std::string files_dir = find_files_dir(pkg);
    g_data_dir = files_dir + "/gridlessdreams";
    g_ext_dir = "/sdcard/Android/data/" + pkg + "/files/gridlessdreams";
    mkdirs(g_data_dir);
    mkdirs(g_ext_dir);
    LOG("Data dir: %s", g_data_dir.c_str());
    LOG("External dir: %s", g_ext_dir.c_str());

    // hardcoded paths first (fast, always available)
    build_hardcoded_dirs(g_search_dirs, pkg);
    // JNI-discovered paths on top (SD cards, adoptable storage, etc.)
    jni_add_context_dirs(g_search_dirs);

    LOG("Total search dirs: %zu", g_search_dirs.size());

    std::string apk = find_apk_path(pkg);
    if (!apk.empty()) {
        LOG("APK: %s", apk.c_str());
        extract_apk_assets(apk, g_data_dir);
    }
}

bool platform::zip_ready() { return !g_zips.empty(); }

void platform::try_open_zip() {
    if (!g_zips.empty()) return;
    // each search dir + its parent (the user may drop the zips one level up)
    std::vector<std::string> dirs;
    for (auto& d : g_search_dirs) {
        dirs.push_back(d);
        std::string parent = parent_dir(d);
        if (parent != d) dirs.push_back(parent);
    }
    open_portion_zips(dirs, "android", jni_open_file);
}

std::string platform::read_file(const std::string& path) {
    auto zdata = zip_read_file(path);
    if (!zdata.empty()) return zdata;

    for (auto& base : g_search_dirs) {
        std::string full = base + "/" + path;
        std::ifstream f(full, std::ios::binary);
        if (f.is_open()) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return ss.str();
        }
    }
    // JNI fallback for permission-denied paths
    for (auto& base : g_search_dirs) {
        std::string full = base + "/" + path;
        auto data = jni_read_file(full);
        if (!data.empty()) return data;
    }
    return "";
}

bool platform::file_exists(const std::string& path) {
    if (zip_has_file(path)) return true;
    for (auto& base : g_search_dirs) {
        std::string full = base + "/" + path;
        if (access(full.c_str(), F_OK) == 0) return true;
    }
    for (auto& base : g_search_dirs) {
        std::string full = base + "/" + path;
        int fd = jni_open_file(full);
        if (fd >= 0) { close(fd); return true; }
    }
    return false;
}

std::string platform::get_data_dir() { return g_data_dir; }

std::string platform::get_writable_dir() {
    // self-heal: if init_android hasn't run (or the dir came back empty) recompute from
    // the hardcoded app data path so the db/cache always land somewhere writable
    if (g_data_dir.empty()) {
        std::string pkg = get_package_name();
        if (!pkg.empty()) {
            g_data_dir = find_files_dir(pkg) + "/gridlessdreams";
            mkdirs(g_data_dir);
            LOG("Recovered writable dir: %s", g_data_dir.c_str());
        }
    }
    return g_data_dir;
}

// ============================================================================
// iOS
// ============================================================================

#elif defined(__APPLE__)

static std::string g_data_dir;
static std::string g_docs_dir;

static std::string find_app_bundle() {
    Dl_info info;
    if (dladdr((void*)find_app_bundle, &info) && info.dli_fname) {
        std::string path(info.dli_fname);
        auto pos = path.rfind("/Frameworks/");
        if (pos != std::string::npos)
            return path.substr(0, pos);
    }
    return ".";
}

static std::string find_documents_dir() {
    const char* home = getenv("HOME");
    if (home)
        return std::string(home) + "/Documents/gridlessdreams";
    std::string bundle = find_app_bundle();
    auto pos = bundle.rfind('/');
    if (pos != std::string::npos)
        return bundle.substr(0, pos) + "/Documents/gridlessdreams";
    return "./gridlessdreams";
}

static bool g_zip_initialized = false;
static void init_ios_zip() {
    if (g_zip_initialized) return;
    g_zip_initialized = true;
    if (g_docs_dir.empty()) g_docs_dir = find_documents_dir();

    std::string writable = platform::get_writable_dir();
    LOG("docs_dir: %s", g_docs_dir.c_str());
    LOG("writable_dir: %s", writable.c_str());

    std::vector<std::string> dirs;
    dirs.push_back(g_docs_dir);
    dirs.push_back(writable);
    auto pos = g_docs_dir.rfind('/');
    if (pos != std::string::npos) dirs.push_back(g_docs_dir.substr(0, pos));
    open_portion_zips(dirs, "ios");
}

std::string platform::read_file(const std::string& path) {
    if (g_data_dir.empty()) g_data_dir = find_app_bundle() + "/gridlessdreams";
    if (g_docs_dir.empty()) g_docs_dir = find_documents_dir();
    init_ios_zip();

    // zip first
    auto zdata = zip_read_file(path);
    if (!zdata.empty()) return zdata;

    // app bundle (small bundled files like masterdata)
    {
        std::ifstream f(g_data_dir + "/" + path, std::ios::binary);
        if (f.is_open()) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return ss.str();
        }
    }
    // Documents dir (extracted assets)
    {
        std::ifstream f(g_docs_dir + "/" + path, std::ios::binary);
        if (f.is_open()) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return ss.str();
        }
    }
    return "";
}

bool platform::file_exists(const std::string& path) {
    if (g_data_dir.empty()) g_data_dir = find_app_bundle() + "/gridlessdreams";
    if (g_docs_dir.empty()) g_docs_dir = find_documents_dir();
    init_ios_zip();
    if (zip_has_file(path)) return true;
    std::ifstream f1(g_data_dir + "/" + path);
    if (f1.good()) return true;
    std::ifstream f2(g_docs_dir + "/" + path);
    return f2.good();
}

std::string platform::get_data_dir() {
    if (g_data_dir.empty()) g_data_dir = find_app_bundle() + "/gridlessdreams";
    return g_data_dir;
}

std::string platform::get_writable_dir() {
    const char* home = getenv("HOME");
    if (home) return std::string(home) + "/Documents";
    if (g_docs_dir.empty()) g_docs_dir = find_documents_dir();
    return g_docs_dir;
}

bool platform::zip_ready() { return !g_zips.empty(); }

void platform::try_open_zip() {
    g_zip_initialized = false;  // force a rescan so a later-dropped zip is picked up
    init_ios_zip();
}

// ============================================================================
// DESKTOP FALLBACK
// ============================================================================

#else

std::string platform::read_file(const std::string& path) {
    auto zdata = zip_read_file(path);
    if (!zdata.empty()) return zdata;
    std::ifstream f("gridlessdreams/" + path, std::ios::binary);
    if (!f.is_open()) return "";
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

bool platform::file_exists(const std::string& path) {
    if (zip_has_file(path)) return true;
    std::ifstream f("gridlessdreams/" + path);
    return f.good();
}

std::string platform::get_data_dir() { return "gridlessdreams"; }
std::string platform::get_writable_dir() { return "gridlessdreams"; }
bool platform::zip_ready() { return !g_zips.empty(); }
void platform::try_open_zip() {}

#endif
