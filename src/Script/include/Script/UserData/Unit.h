#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Unit
    {
        using type = ::UnitPtr;
        static const char* MT_NAME;
        static const luaL_Reg methods[];

        static int CalculateMovement(lua_State* luaState);
    };
}