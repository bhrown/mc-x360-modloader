// module.h

#pragma once

#include "macros.h"

extern "C" int __cdecl DbgPrint(const char* format, ...);

struct Vec3_double {
    double x, y, z;
};

#define Item_sword_wood     GLOBAL(void*, 0x82A01234)
#define Item_sword_stone    GLOBAL(void*, 0x82A01238)
#define Minecraft_instance  GLOBAL(void*, 0x82A05678)

DECL_FUNC(Vec3_double*, Vec3_newTemp_double_double_double, double x, double y, double z)
#define aVec3_newTemp_double_double_double 0x825A2E08





