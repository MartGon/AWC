#pragma once

#include <lua.hpp>

#include <string>

namespace Script::UserData
{
    enum class DatumType : uint8_t
    {
        REFERENCE,
        COPY
    };

    template<typename T>
    struct UserDatum
    {
        T* ptr;
        DatumType type;
    };

    namespace UserData
    {
        template <typename T>
        void RegisterMetatable(lua_State* luaState)
        {
            luaL_newmetatable(luaState, T::MT_NAME);

            lua_pushstring(luaState, "__index");
            lua_pushvalue(luaState, -2);
            lua_settable(luaState, -3); // Metatable.__index = Metatable

            luaL_setfuncs(luaState, T::methods, 0);
            lua_pop(luaState, 1);
        }

        template <typename T>
        void RegisterLib(lua_State* luaState)
        {
            lua_newtable(luaState);
            luaL_setfuncs(luaState, T::functions, 0);
            lua_setglobal(luaState, T::LIB_NAME);
        }

        template <typename T>
        void RegisterMetaMethod(lua_State* luaState, const char* key, const lua_CFunction method)
        {
            luaL_getmetatable(luaState, T::MT_NAME);
            lua_pushcfunction(luaState, method);
            lua_setfield(luaState, -2, key);
            lua_pop(luaState, 1);
        }
        
        template <typename T>
        int Delete(lua_State* luaState)
        {
            auto userDatum = static_cast<UserDatum<T>*>(lua_touserdata(luaState, -1));
            if(userDatum->type == DatumType::COPY)
                delete static_cast<T*>(userDatum->ptr);

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
            auto userdatum = static_cast<UserDatum<type>*>(luaL_checkudata(luaState, index, mtName.c_str()));
            auto userdata = userdatum->ptr;
            std::string error{mtName + " expected"};
            luaL_argcheck(luaState, userdata != nullptr, index, error.c_str());

            return userdata;
        }

        template <typename T>
        typename T::type* PushDataCopy(lua_State* luaState, typename T::type value)
        {
            using type = typename T::type;
            auto datum = static_cast<UserDatum<type>*>(lua_newuserdata(luaState, sizeof(UserDatum<type>)));
            datum->type = DatumType::COPY;
            datum->ptr = new type(value);

            if(!HasGCMethod(luaState, T::MT_NAME))
                AddGCMethod<type>(luaState, T::MT_NAME);

            luaL_setmetatable(luaState, T::MT_NAME);

            return datum->ptr;
        }

        template <typename T>
        void PushDataRef(lua_State* luaState, typename T::type* userdata)
        {
            using type = typename T::type;
            auto datum = static_cast<UserDatum<type>*>(lua_newuserdata(luaState, sizeof(UserDatum<type>)));
            datum->type = DatumType::REFERENCE;
            datum->ptr = userdata;

            luaL_setmetatable(luaState, T::MT_NAME);
        }
    } 
}