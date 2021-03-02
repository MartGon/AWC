#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData::UnitMovement
{
    extern const char* MT_NAME;

    extern const luaL_Reg methods[];

    void Init(lua_State* luaState);
    int Delete(lua_State* luaState);

    int CanMove(lua_State* luaState);
}