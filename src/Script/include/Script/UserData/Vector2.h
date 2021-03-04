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
    int New(lua_State* luaState);

    int Get(lua_State* luaState);
    int Set(lua_State* luaState);
    
    int Add(lua_State* luaState);
    int Sub(lua_State* luaState);
    int Eq(lua_State* luaState);
}