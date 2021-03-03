#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Game
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];

    int GetMap(lua_State* L);
    int GetMapCount(lua_State* L);
}