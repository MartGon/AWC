#pragma once

#include <lua.hpp>

#include <string>

namespace Script::UserData::UserData
{
    void RegisterMetatable(lua_State* luaState, const char* mtName, const luaL_Reg* methods);
    void RegisterMetaMethod(lua_State* luaState, const char* mtName, const char* key, const lua_CFunction method);
    void RegisterLib(lua_State* luaState, const char* libName, const luaL_Reg* funcs);
    
    template <typename T>
    int Delete(lua_State* luaState)
    {
        delete *static_cast<T**>(lua_touserdata(luaState, -1));

        return 0;
    }

    bool HasGCMethod(lua_State* luaState, const char* mtName);
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
    typename T::type* ToUserData(lua_State* luaState, int index)
    {
        using type = typename T::type;
        std::string mtName = T::MT_NAME;
        auto userdata = static_cast<type*>(*static_cast<type**>(luaL_checkudata(luaState, index, mtName.c_str())));
        std::string error{mtName + " expected"};
        luaL_argcheck(luaState, userdata != nullptr, index, error.c_str());

        return userdata;
    }

    template <typename T>
    typename T::type* PushGCData(lua_State* luaState, typename T::type value)
    {
        using type = typename T::type;
        type** ptr = static_cast<type**>(lua_newuserdata(luaState, sizeof(type*)));
        *ptr = new type(value);

        if(!HasGCMethod(luaState, T::MT_NAME))
            AddGCMethod<T>(luaState, T::MT_NAME);

        luaL_setmetatable(luaState, T::MT_NAME);

        return *ptr;
    }

    template <typename T>
    void PushDataRef(lua_State* luaState, typename T::type* userdata)
    {
        using Type = typename T::type;
        Type** ptr = static_cast<Type**>(lua_newuserdata(luaState, sizeof(void*)));
        *ptr = userdata;

        luaL_setmetatable(luaState, T::MT_NAME);
    }    
}