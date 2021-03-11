#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::TilePatternDesc
{
    extern const char* MT_NAME;
    extern const char* LIB_NAME;

    extern const luaL_Reg methods[];
    extern const luaL_Reg functions[];

    int New(lua_State* luaState);

}