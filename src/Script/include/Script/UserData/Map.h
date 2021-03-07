#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Map
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];

    void CheckMapPosition(lua_State* luaState, ::Map* map, ::Vector2 pos);

    int GetUnit(lua_State* L);
    int RemoveUnit(lua_State* L);
    int AddUnit(lua_State* L);
}