// loader.cpp
#include "loader.h"
// #include <xtl.h>

// extern "C" int __cdecl DbgPrint(const char* format, ...);

void loadMod() {
    IMPORT_FUNC(LoadLibrary, aLoadLibrary)
    char path[32];
    // "game:\module.xex" constructed at runtime
    path[0]  = 'g';
    path[1]  = 'a';
    path[2]  = 'm';
    path[3]  = 'e';
    path[4]  = ':';
    path[5]  = '\\';
    path[6]  = 'm';
    path[7]  = 'o';
    path[8]  = 'd';
    path[9]  = 'u';
    path[10] = 'l';
    path[11] = 'e';
    path[12] = '.';
    path[13] = 'x';
    path[14] = 'e';
    path[15] = 'x';
    path[16] = '\0';
    mod = LoadLibrary(path);
}

void dispatcher() {
    unsigned int caller;
    unsigned int saved_r3;
    unsigned int fn_addr;

    __asm {
        mflr r11
        stw  r11, caller
        stw  r3,  saved_r3
    }

    IMPORT_FUNC(GetProcAddress, aGetProcAddress)
    int ordinal = ((int (*)(unsigned int))GetProcAddress(mod, (LPCSTR)1))(caller);
    fn_addr = (unsigned int)GetProcAddress(mod, (LPCSTR)ordinal);

    __asm {
        lwz   r3,  saved_r3   // restore thisptr
        lwz   r12, fn_addr    // load function pointer
        lwz   r11, caller     // load saved LR
        mtlr  r11             // restore LR
        addi  r1, r1, 0x80    // tear down stack frame
        mtctr r12             // load fn ptr into CTR
        bctr                  // jump - returns directly to original caller
    }
}

// void main() {
//     auto* f = (void (*)(void))dispatcher();
//     DbgPrint("Calling module function\n");
//     f();
// }
