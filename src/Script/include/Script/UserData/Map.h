#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Map
{
    void Init(lua_State* L);
    void Push(lua_State* L, ::Map* map);

    int GetUnit(lua_State* L);
}