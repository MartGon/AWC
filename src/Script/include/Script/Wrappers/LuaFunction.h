#pragma once

#include <lua.hpp>

#include <Script/Utils/Utils.h>

#include <AWC.h>

namespace Script
{
    class LuaFunction
    {
    public:
        // Creates a Wrapper to a nil function
        LuaFunction(lua_State* luaState);

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

        // Copy operations
        LuaFunction(const LuaFunction&);
        LuaFunction& operator=(const LuaFunction&);
        friend void swap(LuaFunction& a, LuaFunction& b)
        {
            using std::swap;

            swap(a.functionRef_, b.functionRef_);
        }

        // Move operations
        LuaFunction(LuaFunction&&);
        LuaFunction& operator=(LuaFunction&&);

        ~LuaFunction();

        int PushFunction() const;

    private:

        void CheckType(int index, int top);

        lua_State* luaState_;
        int functionRef_;
    };
}