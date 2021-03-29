#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Process
    {
        using type = ::Process::Process;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int Get(lua_State* luaState);
    };
}