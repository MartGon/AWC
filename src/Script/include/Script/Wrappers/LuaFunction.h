#pragma once

#include <lua.hpp>

#include <Script/Utils/Utils.h>

#include <AWC.h>

namespace Script
{
    template<Scope s>
    class LuaFunction
    {
    public:
        // Creates a Wrapper to a nil function
        LuaFunction(lua_State* luaState) : luaState_{luaState}, functionRef_{LUA_REFNIL}
        {

        }

        // Creates a Wrapper to function at the given index
        LuaFunction(lua_State* luaState, int index) : luaState_{luaState}
        {
            CheckArg<s>(luaState, IsFunction(luaState, index), index, "Function");
            lua_pushvalue(luaState, index);
            functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Creates a function for a wrapper located at t[tIndex], where t is the table at sIndex
        template <typename K>
        LuaFunction(lua_State* luaState, int sIndex, K tIndex) : luaState_{luaState}
        {
            CheckExpectedArg<s>(luaState, IsTable(luaState, sIndex), sIndex, "LuaTable");
            int top = lua_gettop(luaState);
            GetField<K>(luaState, sIndex, tIndex);
            CheckExpectedArgInTable<s>(luaState, IsFunction(luaState, -1), sIndex, tIndex, "LuaFunction", 
                [top](lua_State* luaState){
                    lua_settop(luaState, top);
                });
            functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Copy operations
        friend void swap(LuaFunction& a, LuaFunction& b);

        LuaFunction(const LuaFunction& other) : luaState_{other.luaState_}, 
            functionRef_{(other.PushInternal(), luaL_ref(other.luaState_, LUA_REGISTRYINDEX))}
        {

        }

        LuaFunction& operator=(const LuaFunction& other)
        {
            LuaFunction temp{other};
            swap(*this, temp);

            return *this;
        }
        
        friend void swap(LuaFunction& a, LuaFunction& b)
        {
            using std::swap;

            swap(a.functionRef_, b.functionRef_);
        }

        // Move operations
        LuaFunction(LuaFunction&& other) : luaState_{other.luaState_}, functionRef_{other.functionRef_}
        {
            other.functionRef_ = LUA_REFNIL;
        }
        LuaFunction& operator=(LuaFunction&& other)
        {
            luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);

            functionRef_ = other.functionRef_;
            luaState_ = other.luaState_;

            other.functionRef_ = LUA_REFNIL;

            return *this;
        }

        ~LuaFunction()
        {
            luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);
        }

        int PushInternal() const
        {
            return lua_rawgeti(luaState_, LUA_REGISTRYINDEX, functionRef_);
        }

    private:

        lua_State* luaState_;
        int functionRef_;
    };
}