// module.cpp

#include <unordered_map>
#include <xtl.h>
#include "module.h"
#include <string>

extern "C" int __cdecl DbgPrint(const char* format, ...);

std::unordered_map<unsigned int, int> addr_map;

template<typename Ret, int Offset, typename Args>
inline Ret vcall(void* obj, Args args)
{
    auto fn = *(Ret (**)(void*, Args))(*(DWORD*)obj + Offset);
    return fn(obj, args);
}

void mc_ModInit() {
    DbgPrint("ModInit: Entry\n");
    addr_map[0x00279AD8] = 3; // MinecraftWorld_RunStaticCtors in Minecraft::main
    // addr_map[0x821F29AC] = 3; // Tile::staticCtor in MinecraftWorld_RunStaticCtors
    // addr_map[0x82553AD8] = 4; // Vec3_newTemp_double_double_double in HellDimension::getFogColor
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

int MinecraftWorld_RunStaticCtors() {
    DbgPrint("MinecraftWorld_RunStaticCtors: Entry\n");
    int v0 = ((int (*)(void))0x825AAB78)(); // Packet::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Packet::staticCtor success\n");
    int v1 = ((int (*)(int))0x825AFE40)(v0); // MaterialColor::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: MaterialColor::staticCtor success\n");
    ((int (*)(int))0x82521288)(v1); // Material::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Material::staticCtor success\n");
    int v2 = ((int (*)(void))0x82514698)(); // Tile::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Tile::staticCtor success\n");
    int v3 = ((int (*)(int))0x826069D0)(v2); // HatchetItem::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: HatchetItem::staticCtor success\n");
    int v4 = ((int (*)(int))0x82606CD0)(v3); // PickaxeItem::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: PickaxeItem::staticCtor success\n");
    int v5 = ((int (*)(int))0x82606F78)(v4); // ShovelItem::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: ShovelItem::staticCtor success\n");
    int v6 = ((int (*)(int))0x825F7150)(v5); // BlockReplacements::staticCtor_void_
    DbgPrint("MinecraftWorld_RunStaticCtors: BlockReplacements::staticCtor_void_ success\n");
    int v7 = ((int (*)(int))0x825A63D0)(v6); // Biome::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Biome::staticCtor success\n");
    int v8 = ((int (*)(int))0x8257A2E0)(v7); // Item::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Item::staticCtor success\n");
    ((int (*)(int))0x825F8F28)(v8); // FurnaceRecipes::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: FurnaceRecipes::staticCtor success\n");
    int v9 = ((int (*)(void))0x825C3D50)(); // Recipes::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Recipes::staticCtor success\n");
    ((int (*)(int))0x82587488)(v9); // Stats::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Stats::staticCtor success\n");
    ((int (*)(void))0x825109D0)(); // ServerLevel::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: ServerLevel::staticCtor success\n");
    ((int (*)(void))0x8259DDD0)(); // Skeleton::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Skeleton::staticCtor success\n");
    ((int (*)(void))0x825DBCC0)(); // PigZombie::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: PigZombie::staticCtor success\n");
    ((int (*)(void))0x8252BC30)(); // TileEntity::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: TileEntity::staticCtor success\n");
    int v10 = ((int (*)(void))0x82566548)(); // EntityIO::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: EntityIO::staticCtor success\n");
    int v11 = ((int (*)(int))0x8260A370)(v10); // MobCategory::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: MobCategory::staticCtor success\n");
    int v12 = ((int (*)(int))0x82579918)(v11); // Item::staticInit_void_
    DbgPrint("MinecraftWorld_RunStaticCtors: Item::staticInit_void_ success\n");
    int v13 = ((int (*)(int))0x82524B98)(v12); // LevelChunk::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: LevelChunk::staticCtor success\n");
    int v14 = ((int (*)(int))0x82510948)(v13);
    DbgPrint("v14=0x%08X\n", v14);
    return v14; // GameType::staticCtor

    //Tile_staticCtor();
    //Recipes_staticCtor();
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
