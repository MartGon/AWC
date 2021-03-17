#pragma once

#include <lua.hpp>

#include <Script/Game.h>

namespace Script::UserData
{
    struct Database
    {
        using type = Script::Game::Database;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int Get(lua_State* luaState);
        static int AddUnitType(lua_State* luaState);
    };
}
