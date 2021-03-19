#pragma once

#include <lua.hpp>

namespace Script
{
    class LuaState
    {
    public:
        LuaState() : L{luaL_newstate()}
        {

        }

        ~LuaState()
        {
            lua_close(L);
        }

        lua_State* L;
    };
}