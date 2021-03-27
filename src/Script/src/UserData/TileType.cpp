#include <Script/UserData/TileType.h>
#include <Script/UserData/UserData.h>

#include <Script/UserData/Player.h>
#include <Script/UserData/Unit.h>
#include <Script/UserData/Tile.h>

using namespace Script;

const char* UserData::TileType::MT_NAME = "AWC_TileType";
const char* UserData::TileType::LIB_NAME = "TileType";
const luaL_Reg UserData::TileType::methods[] = {
    {"CreateTile", TileType::CreateTile},
    {NULL, NULL}
};
const luaL_Reg UserData::TileType::functions[] = {
    {NULL, NULL}
};

int UserData::TileType::CreateTile(lua_State* luaState)
{
    auto tileType = UserData::CheckUserData<TileType>(luaState, 1);

    auto tile = tileType->CreateTile();
    UserData::PushDataCopy<Tile>(luaState, tile);

    return 1;
}
