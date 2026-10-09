// SSL bypass: hooks UnityWebRequest::SetUrl to rewrite https -> http.
// both platforms use static binary patching via data slot trampoline (applied by patch.py).
// at runtime, reads hooks.json for data slot RVA and writes hook function pointer.

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>

#include "hook_slot.h"

#ifdef __ANDROID__
#include <android/log.h>
#include <link.h>
#include <elf.h>
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "[UTSK GD]", __VA_ARGS__)
#elif defined(__APPLE__)
#include <os/log.h>
#include <mach-o/dyld.h>
#define LOG(fmt, ...) do { char _logbuf[1024]; snprintf(_logbuf, sizeof(_logbuf), fmt, ##__VA_ARGS__); os_log(OS_LOG_DEFAULT, "[UTSK GD] %{public}s", _logbuf); } while(0)
#endif

#include "json.hpp"
#include "platform.h"
#include "asset_stream.h"

// ===== IL2CPP TYPES =====

struct Il2CppString {
    void* klass;
    void* monitor;
    int32_t length;
    uint16_t chars[1];
};

// ===== IL2CPP STRING HELPERS =====

static bool il2cpp_str_starts_with(Il2CppString* s, const char* prefix) {
    if (!s) return false;
    int plen = (int)strlen(prefix);
    if (s->length < plen) return false;
    for (int i = 0; i < plen; i++)
        if (s->chars[i] != (uint16_t)(uint8_t)prefix[i]) return false;
    return true;
}

static void rewrite_https_to_http(Il2CppString* s) {
    if (!s || s->length < 8) return;
    const uint16_t https[] = {'h','t','t','p','s',':','/','/'};
    for (int i = 0; i < 8; i++)
        if (s->chars[i] != https[i]) return;
    for (int i = 4; i < s->length - 1; i++)
        s->chars[i] = s->chars[i + 1];
    s->chars[s->length - 1] = 0;
    s->length--;
}

// ===== HOOKS =====

// SetUrl hook — rewrites https -> http
typedef void (*seturl_t)(void*, Il2CppString*, void*);
static seturl_t orig_seturl = nullptr;
static int hook_count = 0;

static void hook_seturl(void* thisPtr, Il2CppString* url, void* method) {
    if (url && il2cpp_str_starts_with(url, "https://")) {
        hook_count++;
        rewrite_https_to_http(url);
        LOG("[ssl] [#%d] rewrote URL to http", hook_count);
    }
    if (orig_seturl) orig_seturl(thisPtr, url, method);
}

// ===== FIND MODULE BASE =====

#ifdef __ANDROID__

struct FindModuleData { const char* name; uintptr_t base; };
static int find_module_cb(struct dl_phdr_info* info, size_t, void* data) {
    auto* d = (FindModuleData*)data;
    if (!info->dlpi_name || !strstr(info->dlpi_name, d->name)) return 0;
    d->base = (uintptr_t)info->dlpi_addr;
    return 1;
}

static uintptr_t find_module_base(const char* lib_name) {
    FindModuleData data = { lib_name, 0 };
    dl_iterate_phdr(find_module_cb, &data);
    return data.base;
}

#else // __APPLE__

static uintptr_t find_module_base(const char* lib_name) {
    uint32_t count = _dyld_image_count();
    for (uint32_t i = 0; i < count; i++) {
        const char* name = _dyld_get_image_name(i);
        if (name && strstr(name, lib_name))
            return (uintptr_t)_dyld_get_image_header(i);
    }
    return 0;
}

#endif

// ===== INIT =====

void ssl_bypass_init() {
#ifdef __ANDROID__
    const char* module_name = "libil2cpp.so";
#else
    const char* module_name = "UnityFramework";
#endif

    LOG("[ssl] initializing (data slots)...");

    // wait until libil2cpp is loaded before touching it. dl_iterate_phdr lists it only
    // once the linker has mapped every segment (incl. the appended trampoline/slot
    // segments). do NOT dlopen it with RTLD_NOW to "confirm" readiness: that forces eager
    // binding, which rewrites libil2cpp's GOT -- already read-only under RELRO -- and
    // faults under houdini. a short settle lets the loader finish before we install.
    uintptr_t base = 0;
    for (int attempt = 0; attempt < 120; attempt++) {
        base = find_module_base(module_name);
        if (base) break;
        if (attempt % 10 == 0)
            LOG("[ssl] waiting for %s... (%d/120)", module_name, attempt + 1);
        usleep(500000);
    }
    if (!base) {
        LOG("[ssl] %s not found after 60s", module_name);
        return;
    }
    usleep(2000000);  // let mapping/relocation/RELRO settle before patching slots
    LOG("[ssl] %s ready @ 0x%lx", module_name, (unsigned long)base);

    std::string hooks_json;
    for (int attempt = 1; ; attempt++) {
        hooks_json = platform::read_file("hooks.json");
        if (!hooks_json.empty()) break;
        if (attempt % 10 == 0)
            LOG("[hooks] waiting for hooks.json... (attempt %d)", attempt);
        usleep(2000000);
    }

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(hooks_json);
    } catch (const std::exception& e) {
        LOG("[ssl] failed to parse hooks.json: %s", e.what());
        return;
    }

    auto& hooks = j["hooks"];
    if (hooks.empty()) {
        LOG("[ssl] no hooks in hooks.json");
        return;
    }

    // raw method addresses libdreams calls directly (not hooked), e.g. get_persistentDataPath
    uintptr_t get_pdp_rva = 0;
    if (j.contains("helpers") && j["helpers"].contains("get_persistentDataPath"))
        get_pdp_rva = (uintptr_t)strtoull(
            j["helpers"]["get_persistentDataPath"].get<std::string>().c_str(), nullptr, 16);

    for (auto& h : hooks) {
        std::string name = h["name"].get<std::string>();
        uintptr_t slot_rva = (uintptr_t)strtoull(
            h["data_slot_rva"].get<std::string>().c_str(), nullptr, 16);
        uintptr_t orig_rva = (uintptr_t)strtoull(
            h["orig_rva"].get<std::string>().c_str(), nullptr, 16);

        if (name == "SetUrl") {
            orig_seturl = (seturl_t)(base + orig_rva);
            write_hook_slot(base + slot_rva, (uintptr_t)hook_seturl);
            LOG("[hooks] SetUrl hooked (orig @ 0x%lx, slot @ 0x%lx)",
                (unsigned long)(base + orig_rva), (unsigned long)(base + slot_rva));
        } else if (name == "ToAssetPath") {
            asset_stream_install(base, orig_rva, slot_rva, get_pdp_rva);
        } else {
            LOG("[hooks] unknown hook: %s", name.c_str());
        }
    }

    LOG("[hooks] %zu hooks applied", hooks.size());
}
