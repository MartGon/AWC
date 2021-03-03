#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData::UnitMovement
{
    extern const char* MT_NAME;

    extern const luaL_Reg methods[];

    int CanMove(lua_State* luaState);
}