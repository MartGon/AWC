#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Game
    {
        using type = ::Game;
        static const char* MT_NAME;
        static const luaL_Reg methods[];

        static int GetMap(lua_State* L);
        static int GetMapCount(lua_State* L);
    };
}