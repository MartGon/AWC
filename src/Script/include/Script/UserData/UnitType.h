#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct UnitType
    {
        using type = ::UnitType;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int CreateUnit(lua_State* luaState);
    };
}