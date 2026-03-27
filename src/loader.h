// loader.h

#pragma once

#include "macros.h"

#define mod_addr 0x82A82514
#define mod (*((HMODULE*)mod_addr))

typedef void* HMODULE;
typedef const char* LPCSTR;
typedef void (*FARPROC)(void);

DECL_FUNC(HMODULE, LoadLibrary, LPCSTR lpLibFileName)
#define aLoadLibrary 0x822EAB30

DECL_FUNC(FARPROC, GetProcAddress, HMODULE hModule, LPCSTR lpProcName)
#define aGetProcAddress 0x822EAA78
