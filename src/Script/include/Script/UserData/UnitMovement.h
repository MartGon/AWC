#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData::UnitMovement
{
    extern const char* MT_NAME;

    extern const luaL_Reg methods[];

    void Init(lua_State* luaState);
    void Push(lua_State* luaState, ::UnitMovement* uMove);
    ::UnitMovement* ToUnitMovement(lua_State* luaState, int index = -1);
}