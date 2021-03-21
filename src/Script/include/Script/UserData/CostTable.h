#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct CostTable
    {
        using type = ::CostTable;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int New(lua_State* luaState);
        static type* FromTable(lua_State* luaState, int index);
    };
}