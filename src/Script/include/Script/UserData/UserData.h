#pragma once

#include <lua.hpp>

#include <string>

namespace Script::UserData::UserData
{
    void RegisterMetatable(lua_State* luaState, const char* mtName, const luaL_Reg* methods);
    void RegisterMetaMethod(lua_State* luaState, const char* mtName, const char* key, const lua_CFunction method);
    void RegisterLib(lua_State* luaState, const char* libName, const luaL_Reg* funcs);
    
    bool HasGCMethod(lua_State* luaState, const char* mtName);

    template <typename T>
    T* ToUserData(lua_State* luaState, std::string mtName, int index)
    {
        T* userdata = static_cast<T*>(*static_cast<T**>(luaL_checkudata(luaState, index, mtName.c_str())));
        std::string error{mtName + " expected"};
        luaL_argcheck(luaState, userdata != nullptr, index, error.c_str());

        return userdata;
    }

    template <typename T>
    void PushRawData(lua_State* luaState, const  char* mtName, T* userdata)
    {
        T** ptr = static_cast<T**>(lua_newuserdata(luaState, sizeof(void*)));
        *ptr = userdata;

        luaL_setmetatable(luaState, mtName);
    }

    template <typename T>
    int Delete(lua_State* luaState)
    {
        delete *static_cast<T**>(lua_touserdata(luaState, -1));

        return 0;
    }

    template <typename T>
    void AddGCMethod(lua_State* luaState, const char* mtName)
    {
        int type = luaL_getmetatable(luaState, mtName);
        if(type == LUA_TTABLE)
        {
            lua_pushcfunction(luaState, Delete<T>);
            lua_setfield(luaState, -2, "__gc");
        }

        lua_pop(luaState, 1);
    }

    template <typename T>
    T* PushGCData(lua_State* luaState, const char* mtName, T value)
    {
        T** ptr = static_cast<T**>(lua_newuserdata(luaState, sizeof(T*)));
        *ptr = new T(value);

        if(!HasGCMethod(luaState, mtName))
            AddGCMethod<T>(luaState, mtName);

        luaL_setmetatable(luaState, mtName);

        return *ptr;
    }
}