#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Tile
    {
        using type = ::TilePtr;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];
    };
}