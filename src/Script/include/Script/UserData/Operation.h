#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Operation
    {
        using type = ::OperationIPtr;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static void ToTable(lua_State* luaState, type& process);

        static int GetArgs(lua_State* luaState);
    };
}