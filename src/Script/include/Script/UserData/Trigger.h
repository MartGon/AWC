#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Trigger
    {
        using type = ::Process::Trigger::Trigger;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int InitLib(lua_State* L);

        static int Get(lua_State* L);

        static void ToTable(lua_State* L, type& trigger);
        static type* FromTable(lua_State* L, int index);
    };
}