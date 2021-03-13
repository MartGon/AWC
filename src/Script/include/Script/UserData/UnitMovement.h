#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct UnitMovement
    {
        static const char* MT_NAME;
        static const luaL_Reg methods[];

        static int CanMove(lua_State* luaState);
    };
}