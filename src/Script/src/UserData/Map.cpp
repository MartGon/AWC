#include <Script/UserData/Map.h>
#include <Script/UserData/UserData.h>
#include <Script/UserData/Unit.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::Map::MT_NAME = "AWC_Map";
const luaL_Reg UserData::Map::methods[] = {
        {"GetUnit", Map::GetUnit},
        {NULL, NULL}
    };

void UserData::Map::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}

void UserData::Map::PushLight(lua_State* luaState, ::Map* map)
{
    UserData::PushLight(luaState, MT_NAME, map);
}

Map* UserData::Map::ToMap(lua_State* luaState, int index)
{
    return UserData::ToLightUserData<::Map>(luaState, MT_NAME, index);
}

int UserData::Map::GetUnit(lua_State* luaState)
{
    auto map = UserData::ToLightUserData<::Map>(luaState, MT_NAME, 1);
    auto pos = UserData::ToFullUserData<::Vector2>(luaState, Vector2::MT_NAME, 2);

    bool validPos = map->IsPositionValid(pos->x, pos->y);
    if(validPos)
    {
        auto unit = map->GetUnit(pos->x, pos->y);

        if(unit)
            UserData::PushLight(luaState, Unit::MT_NAME, unit.get());
        else
            lua_pushnil(luaState);
    }
    else
        luaL_error(luaState, "Map position (%d, %d) is not valid", pos->x, pos->y);

    return 1;
}