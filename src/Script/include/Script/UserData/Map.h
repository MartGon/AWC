#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Map
{
    void Init(lua_State* L);
    void Push(lua_State* L, ::Map* map);
    ::Map* ToMap(lua_State* L, int index = -1);

    int GetUnit(lua_State* L);
}