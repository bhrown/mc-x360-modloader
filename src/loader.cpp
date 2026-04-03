// loader.cpp
#include "loader.h"
// #include <xtl.h>

// extern "C" int __cdecl DbgPrint(const char* format, ...);

void loadMod() {
    IMPORT_FUNC(LoadLibrary, aLoadLibrary)
    char path[2];
    // "game:\module.xex" constructed at runtime
    path[0]  = 'z';
    path[1] = '\0';
    mod = LoadLibrary(path);
    IMPORT_FUNC(GetProcAddress, aGetProcAddress)
    ((void (*)(void))GetProcAddress(mod, (LPCSTR)1))();
}

int dispatcher() {
    unsigned int caller;
    unsigned int saved_r3;
    unsigned int fn_addr;

    __asm {
        // LR was saved by compiler prologue at var_8(r1) before stwu
        // After stwu -0x80, that slot is now at 0x78(r1)
        // This works for both vftable calls and direct bl patches
        lwz  r11, 0x78(r1)   // read LR saved by prologue
        stw  r11, caller
        stw  r3,  saved_r3
    }

    IMPORT_FUNC(GetProcAddress, aGetProcAddress)
    int ordinal = ((int (*)(unsigned int))GetProcAddress(mod, (LPCSTR)2))(caller);
    fn_addr = (unsigned int)GetProcAddress(mod, (LPCSTR)ordinal);

    __asm {
        lwz   r3,  saved_r3
        lwz   r12, fn_addr
        lwz   r11, caller
        mtlr  r11
        addi  r1, r1, 0x80
        mtctr r12
        bctr
    }
}

void* dispatcher_noarg() {
    unsigned int caller;
    unsigned int saved_r8;
    void* fn_addr;
    __asm {
        lwz  r11, 0xD8(r1)   // read LR from caller's frame (0x80 + 0x60 - 8)
        stw  r11, caller
    }

    IMPORT_FUNC(GetProcAddress, aGetProcAddress)
    int ordinal = ((int (*)(unsigned int))GetProcAddress(mod, (LPCSTR)2))(caller);
    fn_addr = (void*)GetProcAddress(mod, (LPCSTR)ordinal);

    return ((void* (*)(void))fn_addr)();
}

// For bl-style calls (e.g. Dimension__getFogColor which has frame -0x60 and saves LR at var_8)
// The real caller LR is saved by the intermediate function at 0x60-8 = 0x58 from its r1
// After dispatcher's own stwu -0x80, that slot is at 0x80+0x58 = 0xD8(r1)
int dispatcher_bl() {
    unsigned int caller;
    unsigned int saved_r3;
    unsigned int fn_addr;
    __asm {
        lwz  r11, 0xD8(r1)   // read LR from caller's frame (0x80 + 0x60 - 8)
        stw  r11, caller
        stw  r3,  saved_r3
    }
    IMPORT_FUNC(GetProcAddress, aGetProcAddress)
    int ordinal = ((int (*)(unsigned int))GetProcAddress(mod, (LPCSTR)2))(caller);
    fn_addr = (unsigned int)GetProcAddress(mod, (LPCSTR)ordinal);
    __asm {
        lwz   r3,  saved_r3
        lwz   r12, fn_addr
        lwz   r11, caller
        mtlr  r11
        addi  r1, r1, 0x80
        mtctr r12
        bctr
    }
}

// void main() {
//     auto* f = (void (*)(void))dispatcher();
//     DbgPrint("Calling module function\n");
//     f();
// }
