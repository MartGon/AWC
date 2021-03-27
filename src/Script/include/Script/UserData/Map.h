#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Map
    {
        using type = ::Map;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];
        
        static void CheckMapPosition(lua_State* luaState, ::Map* map, ::Vector2 pos);

        static int GetUnit(lua_State* L);
        static int RemoveUnit(lua_State* L);
        static int AddUnit(lua_State* L);

        static int Fill(lua_State* L);
    };
}