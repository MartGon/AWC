#include <Script/UserData.h>

using namespace Script;

const char* UserData::Map::MT_NAME = "AWC_Map";
const char* UserData::Map::LIB_NAME = "Map";
const luaL_Reg UserData::Map::methods[] = {
        {"GetSize", Map::GetSize},
        {"IsPosValid", Map::IsPosValid},
        {"IsPosFree", Map::IsPosFree},
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

int UserData::Map::GetSize(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto size = map->GetSize();

    UserData::PushDataCopy<Vector2>(luaState, size);

    return 1;
}

int UserData::Map::IsPosValid(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto pos = UserData::CheckUserData<Vector2>(luaState, 2);

    lua_pushboolean(luaState, map->IsPositionValid(*pos));

    return 1;
}

int UserData::Map::IsPosFree(lua_State* luaState)
{
    auto map = UserData::CheckUserData<Map>(luaState, 1);
    auto pos = UserData::CheckUserData<Vector2>(luaState, 2);

    lua_pushboolean(luaState, map->IsPositionFree(*pos));

    return 1;
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
    CheckArg<Scope::Internal>(luaState, map->IsPositionFree(pos), 2, "Pos " + pos.ToString() + " is not free!");
    
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