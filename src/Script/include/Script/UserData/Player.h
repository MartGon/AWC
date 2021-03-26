#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Player
    {
        using type = ::Player;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int GetTeamId(lua_State* luaState);
    };
}