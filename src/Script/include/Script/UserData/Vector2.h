#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Vector2
{
    void Init(lua_State* luaState);
    void Push(lua_State* luaState, ::Vector2* v);
    int New(lua_State* luaState);
    ::Vector2* ToVector2(lua_State* luaState, int index = -1);
}