#pragma once

#include <lua.hpp>

#include <Script/Utils/Utils.h>

#include <AWC.h>

namespace Script
{
    class LuaFunction
    {
    public:
        // Creates a Wrapper to function at the given index
        LuaFunction(lua_State* luaState, int index);

        // Creates a function for a wrapper located at t[tIndex], where t is the table at sIndex
        template <typename K>
        LuaFunction(lua_State* luaState, int sIndex, K tIndex) : luaState_{luaState}
        {
            auto type = lua_type(luaState, sIndex);
            if(type == LUA_TTABLE)
            {
                int top = lua_gettop(luaState);
                type = GetField<K>(luaState, sIndex, tIndex);
                CheckType(-1, top);
                functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
            }
            else
                throw AWCException("LuaFunction: No table found at " + std::to_string(sIndex));
        }

        // Creates a Wrapper to the function a the top of the stack
        LuaFunction(lua_State* luaState) : LuaFunction(luaState, -1)
        {

        }

        LuaFunction(const LuaFunction&);
        LuaFunction(LuaFunction&&);

        ~LuaFunction();

        void PushFunction() const;

    private:

        void CheckType(int index, int top);

        lua_State* luaState_;
        int functionRef_;
    };
}