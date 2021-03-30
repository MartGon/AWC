#pragma once

#include <lua.hpp>

#include <string>

#include <AWC.h>

#include <Script/Utils/Utils.h>


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
        int Delete(lua_State* luaState)
        {
            auto userDatum = static_cast<UserDatum<T>*>(lua_touserdata(luaState, -1));
            if(userDatum->type == DatumType::COPY)
                delete static_cast<T*>(userDatum->ptr);

            return 0;
        }

        template <typename T>
        void RegisterMetatable(lua_State* luaState)
        {
            luaL_newmetatable(luaState, T::MT_NAME);

            // Set __index
            lua_pushstring(luaState, "__index");
            lua_pushvalue(luaState, -2);
            lua_settable(luaState, -3); // Metatable.__index = Metatable

            auto top = lua_gettop(luaState);

            // Set __gc
            lua_pushcfunction(luaState, Delete<typename T::type>);
            lua_setfield(luaState, -2, "__gc"); // Metatable.__gc = Metatable

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

        // Private 
        namespace 
        {
            template <typename T>
            typename T::type* ToUserData(lua_State* luaState, int index)
            {
                return static_cast<UserDatum<typename T::type>*>(lua_touserdata(luaState, index))->ptr;
            }

            template <typename T, Scope s>
            typename T::type* ToUserDataFromTable(lua_State* luaState, int index)
            {
                using type = typename T::type;
                CheckExpectedArg<s>(luaState, IsTable(luaState, index), index, "table");
                type* userdata = T::FromTable(luaState, index);
                lua_pop(luaState, 1);

                return userdata;
            }

            template<typename T, Scope s>
            auto CastOrCreateImp(lua_State* luaState, int index, int) -> decltype(T::FromTable(luaState, index))
            {
                return lua_type(luaState, index) == LUA_TUSERDATA ? 
                    ToUserData<T>(luaState, index) :
                    ToUserDataFromTable<T, s>(luaState, index);
            }

            template<typename T, Scope s>
            auto CastOrCreateImp(lua_State* luaState, int index, double) -> typename T::type*
            {
                return ToUserData<T>(luaState, index);
            }

            template<typename T, Scope s>
            typename T::type* CastOrCreate(lua_State* luaState, int index)
            {
                return CastOrCreateImp<T, s>(luaState, index, 0);
            }
        }

        template <typename T>
        bool IsUserData(lua_State* luaState, int index)
        {
            bool isUD = false;
            auto userdatum = lua_touserdata(luaState, index);
            if(userdatum)
            {
                bool hasMeta = lua_getmetatable(luaState, index);
                luaL_getmetatable(luaState, T::MT_NAME);
                if(hasMeta)
                    isUD = lua_rawequal(luaState, -1, -2);

                lua_pop(luaState, 2);
            }

            return isUD;
        }

        template<typename T>
        bool IsUserDataOrTable(lua_State* luaState, int index)
        {
            return IsUserData<T>(luaState, index) || IsTable(luaState, index);
        }
    
        template<typename T, Scope s = Scope::Internal>
        typename T::type* CheckUserData(lua_State* luaState, int index)
        {   
            CheckExpectedArg<s>(luaState, IsUserDataOrTable<T>(luaState, index), index, T::MT_NAME);
            return CastOrCreate<T, s>(luaState, index);
        }

        template <typename T>
        typename T::type* PushDataCopy(lua_State* luaState, typename T::type value)
        {
            using type = typename T::type;
            auto datum = static_cast<UserDatum<type>*>(lua_newuserdata(luaState, sizeof(UserDatum<type>)));
            datum->type = DatumType::COPY;
            datum->ptr = new type(value);

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