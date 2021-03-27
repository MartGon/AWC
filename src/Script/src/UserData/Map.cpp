#include <Script/UserData.h>

using namespace Script;

const char* UserData::Map::MT_NAME = "AWC_Map";
const char* UserData::Map::LIB_NAME = "Map";
const luaL_Reg UserData::Map::methods[] = {
        {"GetUnit", Map::GetUnit},
        {"RemoveUnit", Map::RemoveUnit},
        {"AddUnit", Map::AddUnit},
        {"Fill", Map::Fill},
        {NULL, NULL}
    };
const luaL_Reg UserData::Map::functions[] = {
    {NULL, NULL}
};

void UserData::Map::CheckMapPosition(lua_State* luaState, ::Map* map, ::Vector2 pos)
{
    bool validPos = map->IsPositionValid(pos.x, pos.y);
    if(!validPos)
    {
        luaL_error(luaState, "Map position %s is not valid", pos.ToString().c_str());
    }
}

int UserData::Map::GetUnit(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto pos = *UserData::CheckUserData<Vector2>(luaState, 2);

    Map::CheckMapPosition(luaState, map, pos);
    auto unit = map->GetUnit(pos);
    if(unit)
        UserData::PushDataCopy<Unit>(luaState, unit);
    else
        lua_pushnil(luaState);

    return 1;
}

int UserData::Map::RemoveUnit(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto pos = *UserData::CheckUserData<Vector2>(luaState, 2);

    Map::CheckMapPosition(luaState, map, pos);
    map->RemoveUnit(pos);

    return 0;
}

int UserData::Map::AddUnit(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto pos = *UserData::CheckUserData<Vector2>(luaState, 2);
    auto unit = *UserData::CheckUserData<Unit>(luaState, 3);

    Map::CheckMapPosition(luaState, map, pos);
    map->AddUnit(pos, unit);

    return 0;
}

int UserData::Map::Fill(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto tileType = UserData::CheckUserData<TileType>(luaState, 2);

    MapUtils::FillMap(*map, *tileType);

    return 0;
}