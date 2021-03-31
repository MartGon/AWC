#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct EventHandler
    {
        using type = Event::Handler;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int New(lua_State* luaState);

        static type* FromTable(lua_State* state, int index);
    };
}