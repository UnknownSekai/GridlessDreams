#pragma once
#include <stdint.h>

// installs the ToAssetPath no-copy hook; called from the hooks.json loop in ssl_bypass.cpp.
// orig_rva/slot_rva come from patch.py's trampoline; get_pdp_rva is get_persistentDataPath.
void asset_stream_install(uintptr_t module_base, uintptr_t orig_rva,
                          uintptr_t slot_rva, uintptr_t get_pdp_rva);
