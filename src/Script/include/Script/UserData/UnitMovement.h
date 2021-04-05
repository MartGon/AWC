#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct UnitMovement
    {
        using type = ::UnitMovement;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int CanMove(lua_State* luaState);
        static int GetTilesPosInReachCount(lua_State* luaState);
        static int GetTilePosInReach(lua_State* L);
    };
}