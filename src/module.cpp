// module.cpp

#include <unordered_map>
#include <xtl.h>
#include "module.h"
#include <string>
#include <cstring>

extern "C" int __cdecl DbgPrint(const char* format, ...);

std::unordered_map<unsigned int, int> addr_map;

template<typename Ret, int Offset, typename Args>
inline Ret vcall(void* obj, Args args) {
    auto fn = *(Ret (**)(void*, Args))(*(DWORD*)obj + Offset);
    return fn(obj, args);
}

template<typename Ret, int Offset>
inline Ret vaccess(void* obj) {
    return *(Ret*)((unsigned char*)obj + Offset);
}

template<typename Ret>
inline Ret vaccess(void* obj, int Offset) {
    return *(Ret*)((unsigned char*)obj + Offset);
}

void print_bytes(unsigned char* p) {
    DbgPrint("quicksoil dump (first 64 bytes):\n");

    for (int i = 0; i < 64; i++) {
        // Print byte
        DbgPrint("%02X ", p[i]);

        // Optional: newline every 16 bytes for readability
        if ((i + 1) % 16 == 0) {
            DbgPrint("\n");
        }
    }
}


int* quicksoil = (int*)malloc(0x78); // v572 = operator new_uint_(0x78u);
void ml_Tile_staticCtor() {
    DbgPrint("ml_Tile_staticCtor: Entry\n");
    int** tiles = (int**)malloc(0x500); // Tile::tiles = (int)operator new___uint_(0x400u);
    int** items = (int**)malloc(0x2F400); // Item::items = (int)operator new___uint_(0x1F400u);
    std::memcpy(tiles, *(int**)(0x829FFF98), 0x400);
    *(int***)(0x829FFF98) = tiles;
    std::memcpy(items, *(int**)(0x82A0DFFC), 0x1F400);
    *(int***)(0x82A0DFFC) = items;
    quicksoil = ((int* (*)(int*, int, int, int))0x825E0FA8)(quicksoil, 99, 104, 1); // v573 = HellSandTile::HellSandTile(v572, 88, 104, 1); // 104 is terrain.png texture index
    quicksoil = vcall<int*, 44>(quicksoil, 0.5); // v574 = (*(int (__fastcall **)(int, double))(*(_DWORD *)v573 + 44))(v573, 0.5);
    quicksoil = vcall<int*, 20>(quicksoil, *(int*)(0x829FFF94)); // v575 = (int (__fastcall ***)(_DWORD, int))(*(int (__fastcall **)(int, int))(*(_DWORD *)v574 + 20))(v574, Tile::SOUND_SAND);
    quicksoil = vcall<int*, 0>(quicksoil, 613); // v576 = (**v575)(v575, 614); // strings.xus item name
    quicksoil = vcall<int*, 308>(quicksoil, 200); // Tile::hellSand = (*(int (__fastcall **)(int, int))(*(_DWORD *)v576 + 308))(v576, 200);
    for (int i = 0; i < 0x100; i++) {
        if (tiles[i] != 0 && items[i] == 0) { // if ( *(_DWORD *)(i + Tile::tiles) && !*(_DWORD *)(Item::items + i) )
            int* obj = (int*)malloc(0x48); // v645 = operator new_uint_(0x48u);
            obj = ((int* (*)(int*, int, int))0x825DEB20)(obj, i - 0x100, 1); // v646 = sub_825DEB20(v645, v643 - 256, 1);
            items[i] = obj; // *(_DWORD *)(Item::items + i) = v646;
        }
    }
    DbgPrint("ml_Tile_staticCtor: Quicksoil initialized");
    DbgPrint("ml_Tile_staticCtor: Return");
}

int* quicksoil_instance = (int*)malloc(0x34u);

int ml_Recipes_staticCtor(int a1) {
    DbgPrint("ml_Recipes_staticCtor: Entry\n");
    // 	addShapedRecipy(new ItemInstance(Tile::wood, 4, 0), //
    //	L"sczg",
    //	L"#", //
    //
    //	L'#', new ItemInstance(Tile::treeTrunk, 1, 0),
    //	L'S');
    DbgPrint("ml_Recipes_staticCtor: check 1\n");
    ((void (*)(int*, int*, int, int))0x825383F8)(quicksoil_instance, quicksoil, 1, 1); // ItemInstance(v125, dword_82A0000C, 1, 1);
    // ((void (*)(int*, int*, int, int))0x825383F8)(quicksoil_instance, *(int**)(0x829FFFDC), 1, 1); // ItemInstance(v125, dword_82A0000C, 1, 1);
    DbgPrint("ml_Recipes_staticCtor: check 2\n");
    ((void (*)(int, int*, const wchar_t*, const wchar_t*, const wchar_t*, int*, const wchar_t*))0x825C13B8)(a1, quicksoil_instance, L"stzg", L"#", L"#", *(int**)(0x829FFFA4), L"S"); // Recipes::addShapedRecipy(a1, v10, &unk_820611FC, &unk_82038870, 35, Tile::treeTrunk, 83);
    ((void (*)(int))0x825C08F0)(a1); // Recipes::buildRecipeIngredientsArray_void_(a1);
    DbgPrint("ml_Recipes_staticCtor: Return\n");
    return a1;
}

void ml_ModInit() {
    DbgPrint("ml_ModInit: Entry\n");
    addr_map[0x00279AD8] = 3; // MinecraftWorld_RunStaticCtors in Minecraft::main
    // addr_map[0x821F29AC] = 3; // Tile::staticCtor in MinecraftWorld_RunStaticCtors
    // addr_map[0x82553AD8] = 4; // Vec3_newTemp_double_double_double in HellDimension::getFogColor
    // addr_map[0x825A2E08] = 4; // Vec3_newTemp_double_double_double in Dimension::getFogColor 
}

int ml_LookupByReturnAddr(unsigned int caller) {
    DbgPrint("ml_LookupByReturnAddr: caller=0x%08X\n", caller);
    auto it = addr_map.find(caller);
    if (it != addr_map.end()) {
        return it->second;
    }
    DbgPrint("ml_LookupByReturnAddr: no match found\n");
    return -1;
}

int MinecraftWorld_RunStaticCtors() {
    DbgPrint("MinecraftWorld_RunStaticCtors: Entry\n");
    int v0 = ((int (*)(void))0x825AAB78)(); // Packet::staticCtor
    int v1 = ((int (*)(int))0x825AFE40)(v0); // MaterialColor::staticCtor
    ((int (*)(int))0x82521288)(v1); // Material::staticCtor
    int v2 = ((int (*)(void))0x82514698)(); // Tile::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Calling ml_Tile_staticCtor ...\n");
    ml_Tile_staticCtor();
    int v3 = ((int (*)(int))0x826069D0)(v2); // HatchetItem::staticCtor
    int v4 = ((int (*)(int))0x82606CD0)(v3); // PickaxeItem::staticCtor
    int v5 = ((int (*)(int))0x82606F78)(v4); // ShovelItem::staticCtor
    int v6 = ((int (*)(int))0x825F7150)(v5); // BlockReplacements::staticCtor_void_
    int v7 = ((int (*)(int))0x825A63D0)(v6); // Biome::staticCtor
    int v8 = ((int (*)(int))0x8257A2E0)(v7); // Item::staticCtor
    ((int (*)(int))0x825F8F28)(v8); // FurnaceRecipes::staticCtor
    int v9 = ((int (*)(void))0x825C3D50)(); // Recipes::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Calling ml_Recipes_staticCtor ...\n");
    int v9_1 = ml_Recipes_staticCtor(v9);
    ((int (*)(int))0x82587488)(v9_1); // Stats::staticCtor
    ((int (*)(void))0x825109D0)(); // ServerLevel::staticCtor
    ((int (*)(void))0x8259DDD0)(); // Skeleton::staticCtor
    ((int (*)(void))0x825DBCC0)(); // PigZombie::staticCtor
    ((int (*)(void))0x8252BC30)(); // TileEntity::staticCtor
    int v10 = ((int (*)(void))0x82566548)(); // EntityIO::staticCtor
    int v11 = ((int (*)(int))0x8260A370)(v10); // MobCategory::staticCtor
    int v12 = ((int (*)(int))0x82579918)(v11); // Item::staticInit_void_
    int v13 = ((int (*)(int))0x82524B98)(v12); // LevelChunk::staticCtor
    int v14 = ((int (*)(int))0x82510948)(v13); // GameType::staticCtor
    DbgPrint("MinecraftWorld_RunStaticCtors: Return\n");
    return v14; // GameType::staticCtor
}

Vec3_double* ml_HellDimension_getFogColor1() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.2, 0.01);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}

Vec3_double* ml_HellDimension_getFogColor2() {
    IMPORT_FUNC(Vec3_newTemp_double_double_double, aVec3_newTemp_double_double_double)
    DbgPrint("calling Vec3::newTemp at 0x%08X\n", aVec3_newTemp_double_double_double);
    Vec3_double* result = Vec3_newTemp_double_double_double(0.01, 0.01, 0.2);
    DbgPrint("result = 0x%08X\n", result);
    return result;
}
