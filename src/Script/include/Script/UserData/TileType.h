#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct TileType
    {
        using type = ::TileType;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int CreateTile(lua_State* luaState);
    };
}