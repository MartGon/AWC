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
        void RegisterMetatable(lua_State* luaState, const char* mtName, const luaL_Reg* methods);
        void RegisterMetaMethod(lua_State* luaState, const char* mtName, const char* key, const lua_CFunction method);
        void RegisterLib(lua_State* luaState, const char* libName, const luaL_Reg* funcs);
        
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