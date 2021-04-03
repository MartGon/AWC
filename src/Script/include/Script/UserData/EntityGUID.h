#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct EntityGUID
    {
        using type = ::Entity::GUID;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static type* FromTable(lua_State* L, int index);
        static void ToTable(lua_State* L, type& type);

        static int Eq(lua_State* L);
        static int Index(lua_State* L);
        static int ToString(lua_State* L);
    };
}