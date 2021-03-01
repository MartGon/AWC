#pragma once

#include <lua.hpp>

#include <string>

namespace Script::UserData::UserData
{
    void RegisterMetatable(lua_State* luaState, const char* mtName, const luaL_Reg* methods);
    void Push(lua_State* luaState, const  char* mtName, void* userdata);

    template <typename T>
    T* ToUserData(lua_State* luaState, int index, std::string mtName)
    {
        T* userdata = static_cast<T*>(luaL_checkudata(luaState, index, mtName.c_str()));
        std::string error{mtName + " expected"};
        luaL_argcheck(luaState, userdata != nullptr, 1, error.c_str());

        return userdata;
    }

    template <typename T>
    T* ToUserData(lua_State* luaState, std::string mtName)
    {
        return ToUserData<T>(luaState, -1, mtName);
    }
}