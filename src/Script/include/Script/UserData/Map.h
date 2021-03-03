#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Map
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];

    int GetUnit(lua_State* L);
}