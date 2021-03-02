#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Vector2
{
    extern const char* MT_NAME;
    extern const char* LIB_NAME;

    extern const luaL_Reg methods[];
    extern const luaL_Reg functions[];

    void Init(lua_State* luaState);
    void PushLight(lua_State* luaState, ::Vector2* v);
    int New(lua_State* luaState);
    ::Vector2* ToVector2(lua_State* luaState, int index = -1);
}