#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Vector2
    {
        using type = ::Vector2;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int New(lua_State* luaState);
        static ::Vector2* FromTable(lua_State* luaState, int index);

        static int Get(lua_State* luaState);
        static int Set(lua_State* luaState);
        
        static int Add(lua_State* luaState);
        static int Sub(lua_State* luaState);
        static int Eq(lua_State* luaState);

        static int ToString(lua_State* luaState);
    };
}