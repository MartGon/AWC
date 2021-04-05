#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct UnitAttack
    {
        using type = ::UnitAttack;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int CanAttack(lua_State* luaState);

        static int GetTilesInRangeCount(lua_State* luaState);
        static int GetTileInRange(lua_State* L);
    };
}