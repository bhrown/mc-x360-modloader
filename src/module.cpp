// module.cpp

#include <unordered_map>
#include <xtl.h>
#include "module.h"

extern "C" int __cdecl DbgPrint(const char* format, ...);

std::unordered_map<unsigned int, int> addr_map;

void mc_ModInit() {
    DbgPrint("ModInit called");
    addr_map[0x82553AD8] = 3; // Vec3_newTemp_double_double_double in HellDimension::getFogColor
    // addr_map[0x825A2E08] = 4; // Vec3_newTemp_double_double_double in Dimension::getFogColor 
}

int mc_LookupByReturnAddr(unsigned int caller) {
    DbgPrint("LookupByReturnAddr: caller=0x%08X\n", caller);
    auto it = addr_map.find(caller);
    if (it != addr_map.end()) {
        return it->second;
    }
    DbgPrint("  no match found\n");
    return -1;
}

Vec3_double* mc_HellDimension_getFogColor1() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.2, 0.01);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}

Vec3_double* mc_HellDimension_getFogColor2() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.01, 0.2);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}
