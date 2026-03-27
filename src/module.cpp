// module.cpp

#include <xtl.h>
#include "module.h"

extern "C" int __cdecl DbgPrint(const char* format, ...);

struct AddrEntry {
    unsigned int caller_addr;
    int          ordinal;
};

static AddrEntry addr_map[] = {
    { 0x82610780, 2 },
    { 0x82553AD8, 3 },
};

int LookupByReturnAddr(unsigned int caller) {
    DbgPrint("LookupByReturnAddr: caller=0x%08X\n", caller);
    for (int i = 0; i < sizeof(addr_map)/sizeof(addr_map[0]); i++) {
        DbgPrint("  checking 0x%08X\n", addr_map[i].caller_addr);
        if (addr_map[i].caller_addr == caller)
            return addr_map[i].ordinal;
    }
    DbgPrint("  no match found\n");
    return -1;
}

Vec3_double* HellDimension_getFogColor1() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.2, 0.01);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}

Vec3_double* HellDimension_getFogColor2() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.01, 0.2);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}
