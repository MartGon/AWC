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
    };
}