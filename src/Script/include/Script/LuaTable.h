#pragma once

#include <lua.hpp>

namespace Script
{
    class ScriptOperation;

    class LuaTable
    {
    friend class Script::ScriptOperation;
    public:
        LuaTable(lua_State* luaState) : luaState_{luaState}
        {
            lua_newtable(luaState);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        LuaTable(lua_State* luaState, std::string mtName) : luaState_{luaState}, mtName_{mtName}
        {
            lua_newtable(luaState);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
            SetMetaTable(mtName);
        }

        ~LuaTable()
        {
            luaL_unref(luaState_, LUA_REGISTRYINDEX, tableRef_);
        }

        void SetInt(std::string key, int n)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            lua_pushinteger(luaState_, n);
            lua_setfield(luaState_, -2, key.c_str());
            lua_pop(luaState_, 1);
        }

        void SetString(std::string key, std::string str)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            lua_pushstring(luaState_, str.c_str());
            lua_setfield(luaState_, -2, key.c_str());
            lua_pop(luaState_, 1);
        }

        int GetInt(std::string key)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            lua_getfield(luaState_, -1, key.c_str());
            int val = lua_tointeger(luaState_, -1);
            lua_pop(luaState_, 2);
            return val;
        }

        std::string GetString(std::string key)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            lua_getfield(luaState_, -1, key.c_str());
            std::string str = std::string(lua_tostring(luaState_, -1));
            lua_pop(luaState_, 2);

            return str;
        }

        template<typename T>
        T* SetFullUserData(std::string key, const char* mtName, T userdata)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            auto ptr = Script::UserData::UserData::PushFullUserData(luaState_, mtName, userdata);
            lua_setfield(luaState_, -2, key.c_str());
            lua_pop(luaState_, 1);

            return ptr;
        }

        template <typename T>
        T* GetFullUserData(std::string key, const char* mtName)
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            lua_getfield(luaState_, -1, key.c_str());
            T* ptr = Script::UserData::UserData::ToFullUserData<T>(luaState_, mtName, -1);
            lua_pop(luaState_, 2);

            return ptr;
        }

        void SetMetaTable(std::string mtName)
        {
            int type = lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            luaL_setmetatable(luaState_, mtName.c_str());
            lua_pop(luaState_, 1);
        }

    private:

        int GetRef()
        {
            return tableRef_;
        }

        lua_State* luaState_;
        std::string mtName_;
        int tableRef_;
    };
}