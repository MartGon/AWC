#include <Script/UserData/Map.h>
#include <Script/UserData/UserData.h>

using namespace Script;

static const char* MT_NAME = "AWC_Map";

const luaL_Reg methods[] = {
                
        {NULL, NULL}
    };

void UserData::Map::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}

void UserData::Map::Push(lua_State* luaState, ::Map* map)
{
    UserData::Push(luaState, MT_NAME, map);
}

Map* UserData::Map::ToMap(lua_State* luaState, int index)
{
    return UserData::ToUserData<::Map>(luaState, index, MT_NAME);
}

int UserData::Map::GetUnit(lua_State* luaState)
{
    auto map = ToMap(luaState);
    int x = luaL_checkinteger(luaState, 2);
    int y = luaL_checkinteger(luaState, 3);

    bool validPos = map->IsPositionValid(x, y);
    luaL_error(luaState, "Map position (%d, %d) is not valid", x, y);

    auto unit = map->GetUnit(x, y);

    // TODO
}