#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct MovementDescType
    {
        using type = ::MovementDescTypePtr;
        static const char* MT_NAME;
        static const char* LIB_NAME;

        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int New(lua_State*);
    };

    
}