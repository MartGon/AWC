#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Game
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];

    void Init(lua_State* L);
    void PushLight(lua_State* L, ::Game* game);

    int GetMap(lua_State* L);
    int GetMapCount(lua_State* L);
}