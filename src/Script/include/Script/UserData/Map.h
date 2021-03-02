#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Map
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];

    void Init(lua_State* L);
    void PushLight(lua_State* L, ::Map* map);
    ::Map* ToMap(lua_State* L, int index = -1);

    int GetUnit(lua_State* L);
}