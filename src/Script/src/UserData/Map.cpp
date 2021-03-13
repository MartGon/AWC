#include <Script/UserData/Map.h>
#include <Script/UserData/UserData.h>
#include <Script/UserData/Unit.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::Map::MT_NAME = "AWC_Map";
const luaL_Reg UserData::Map::methods[] = {
        {"GetUnit", Map::GetUnit},
        {"RemoveUnit", Map::RemoveUnit},
        {"AddUnit", Map::AddUnit},
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
    auto map = UserData::ToUserData<Map>(luaState, 1);
    auto pos = *UserData::ToUserData<Vector2>(luaState, 2);

    Map::CheckMapPosition(luaState, map, pos);
    auto unit = map->GetUnit(pos);
    if(unit)
        UserData::PushGCData<Unit>(luaState, unit);
    else
        lua_pushnil(luaState);

    return 1;
}

int UserData::Map::RemoveUnit(lua_State* luaState)
{
    auto map = UserData::ToUserData<Map>(luaState, 1);
    auto pos = *UserData::ToUserData<Vector2>(luaState, 2);

    Map::CheckMapPosition(luaState, map, pos);
    map->RemoveUnit(pos);

    return 0;
}

int UserData::Map::AddUnit(lua_State* luaState)
{
    auto map = UserData::ToUserData<Map>(luaState, 1);
    auto pos = *UserData::ToUserData<Vector2>(luaState, 2);
    auto unit = *UserData::ToUserData<Unit>(luaState, 3);

    Map::CheckMapPosition(luaState, map, pos);
    map->AddUnit(pos, unit);

    return 0;
}