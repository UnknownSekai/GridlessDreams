#pragma once
#include <stdint.h>
#include <sys/mman.h>

// store a hook function pointer into a data slot that patch.py placed in an appended
// R+W segment of the target binary. a native-bridge translator (houdini) can map that
// segment read-only despite the W flag, so force the slot's page writable first; the
// store is 8-byte aligned inside the page, so one page always covers it.
static inline void write_hook_slot(uintptr_t slot_addr, uintptr_t fn) {
    uintptr_t page = slot_addr & ~(uintptr_t)0xFFF;
    mprotect((void*)page, 0x1000, PROT_READ | PROT_WRITE);
    *(uintptr_t*)slot_addr = fn;
}
