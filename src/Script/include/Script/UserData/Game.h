#pragma once

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData
{
    struct Game
    {
        using type = ::Game;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int CreateMap(lua_State* L);
        static int GetMap(lua_State* L);
        static int GetMapCount(lua_State* L);

        static int CreatePlayer(lua_State* L);

        static int CancelProcess(lua_State* L);
        static int GetHistoryCount(lua_State* L);
        static int GetHistoryProcess(lua_State* L);
    };
}