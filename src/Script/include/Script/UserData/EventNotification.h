#pragma once

#include <lua.hpp>

#include <AWC.h>

namespace Script::UserData
{
    struct EventNotification
    {
        using type = Event::Notification::Notification;
        static const char* MT_NAME;
        static const char* LIB_NAME;
        static const luaL_Reg methods[];
        static const luaL_Reg functions[];

        static int InitLib(lua_State* L);
        static void ToTable(lua_State* L, type& notification);
    };
}