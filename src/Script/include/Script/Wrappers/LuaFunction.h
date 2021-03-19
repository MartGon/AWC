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
        LuaFunction(lua_State* luaState, int sIndex, K tIndex)
        {
            auto type = GetField<K>(luaState, sIndex, tIndex);
            if(type == LUA_TFUNCTION)
            {
                functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
            }
            else
                throw AWCException("LuaFunction: value at index " + std::to_string(sIndex) + " was not a function");
        }

        // Creates a Wrapper to the function a the top of the stack
        LuaFunction(lua_State* luaState) : LuaFunction(luaState, -1)
        {

        }

        LuaFunction(const LuaFunction&) = delete;
        LuaFunction(LuaFunction&&) = default;

        ~LuaFunction();

        void PushFunction() const;

    private:

        lua_State* luaState_;
        unsigned int functionRef_;
    };
}