#pragma once

#include <AWC.h>

#include <lua.hpp>

#include <Script/UserData/UserData.h>

namespace Script
{
    class ScriptOperation;

    class LuaTable
    {
    friend class Script::ScriptOperation;
    public:
        // Creates a Wrapper for a table at the top of the stack
        LuaTable(lua_State* luaState, int index) : luaState_{luaState}
        {
            lua_pushvalue(luaState, index);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Creates a Wrapper for a new table
        LuaTable(lua_State* luaState) : luaState_{luaState}
        {
            lua_newtable(luaState);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Creates a Wrapper for a new table, with the given metatable.
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
            PushLuaTable();
            lua_pushinteger(luaState_, n);
            SetField(key);
        }

        void SetInt(int index, int n)
        {
            PushLuaTable();
            lua_pushinteger(luaState_, n);
            SetField(index);
        }

        int GetInt(std::string key)
        {
            PushLuaTable();
            lua_getfield(luaState_, -1, key.c_str());
            int val = lua_tointeger(luaState_, -1);
            lua_pop(luaState_, 2);
            return val;
        }

        int GetInt(int index)
        {
            PushLuaTable();

            lua_geti(luaState_, -1, index);
            int val = lua_tointeger(luaState_, -1);
            lua_pop(luaState_, 2);

            return val;
        }

        void SetString(std::string key, std::string str)
        {
            PushLuaTable();
            lua_pushstring(luaState_, str.c_str());
            SetField(key);
        }

        void SetString(int index, std::string str)
        {
            PushLuaTable();
            lua_pushstring(luaState_, str.c_str());
            SetField(index);
        }

        std::string GetString(std::string key)
        {
            PushLuaTable();
            lua_getfield(luaState_, -1, key.c_str());
            std::string str = std::string(lua_tostring(luaState_, -1));
            lua_pop(luaState_, 2);

            return str;
        }

        std::string GetString(int index)
        {
            PushLuaTable();
            lua_geti(luaState_, -1, index);
            std::string str = std::string(lua_tostring(luaState_, -1));
            lua_pop(luaState_, 2);

            return str;
        }

        void SetBool(std::string key, bool b)
        {
            PushLuaTable();
            lua_pushboolean(luaState_, b);
            SetField(key);
        }

        void SetBool(int index, bool b)
        {
            PushLuaTable();
            lua_pushboolean(luaState_, b);
            SetField(index);
        }

        bool GetBool(std::string key)
        {
            PushLuaTable();
            lua_getfield(luaState_, -1, key.c_str());
            bool b = lua_toboolean(luaState_, -1);
            lua_pop(luaState_, 2);

            return b;
        }

        bool GetBool(int index)
        {
            PushLuaTable();
            lua_geti(luaState_, -1, index);
            bool b = lua_toboolean(luaState_, -1);
            lua_pop(luaState_, 2);

            return b;
        }

        std::optional<LuaTable> GetTable(std::string key)
        {
            PushLuaTable();
            lua_getfield(luaState_, -1, key.c_str());
            
            return LuaTable{luaState_, 1};
        }

        std::optional<LuaTable> GetTable(int index)
        {
            PushLuaTable();
            lua_geti(luaState_, -1, index);

            return LuaTable{luaState_, 1};
        }

        template <typename T>
        T* GetUserData(std::string key, const char* mtName)
        {
            PushLuaTable();
            lua_getfield(luaState_, -1, key.c_str());
            T* ptr = Script::UserData::UserData::ToUserData<T>(luaState_, mtName, -1);
            lua_pop(luaState_, 2);

            return ptr;
        }

        template <typename T>
        T* GetUserData(int index, const char* mtName)
        {
            PushLuaTable();
            lua_geti(luaState_, -1, index);
            T* ptr = Script::UserData::UserData::ToUserData<T>(luaState_, mtName, -1);
            lua_pop(luaState_, 2);

            return ptr;
        }

        template<typename T>
        T* SetGCData(std::string key, const char* mtName, T userdata)
        {
            PushLuaTable();
            auto ptr = Script::UserData::UserData::PushGCData(luaState_, mtName, userdata);
            SetField(key);

            return ptr;
        }

        template <typename T>
        T* SetGCData(int index, const char* mtName, T userdata)
        {
            PushLuaTable();
            auto ptr = Script::UserData::UserData::PushGCData(luaState_, mtName, userdata);
            SetField(index);

            return ptr;
        }

        template<typename T>
        void SetRawData(std::string key, const char* mtName, T userdata)
        {
            PushLuaTable();
            Script::UserData::UserData::PushRawData(luaState_, mtName, userdata);
            SetField(key);
        }

        template <typename T>
        void SetRawData(int index, const char* mtName, T userdata)
        {
            PushLuaTable();
            Script::UserData::UserData::PushRawData(luaState_, mtName, userdata);
            SetField(index);
        }

        void SetMetaTable(std::string mtName)
        {
            PushLuaTable();
            luaL_setmetatable(luaState_, mtName.c_str());
            lua_pop(luaState_, 1);
        }

        bool ContainsValue(std::string key)
        {
            PushLuaTable();
            int type = lua_getfield(luaState_, -1, key.c_str());
            
            return type != LUA_TNIL;
        }

        int Length()
        {
            PushLuaTable();
            int len = luaL_len(luaState_, -1);
            lua_pop(luaState_, 1);

            return len;
        }

    private:

        void PushLuaTable()
        {
            int type = lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
            if(type != LUA_TTABLE)
                throw AWCException("LuaTable: Internal Lua table was not type LUA_TTABLE");
        }

        // Assumes that table is at -2 and value is at the top of the stack
        // Pops the internal lua table from the stack
        void SetField(std::string key)
        {
            lua_setfield(luaState_, -2, key.c_str());
            lua_pop(luaState_, 1);
        }

        void SetField(int index)
        {
            lua_seti(luaState_, -2, index);
            lua_pop(luaState_, 1);
        }

        int GetRef()
        {
            return tableRef_;
        }

        lua_State* luaState_;
        std::string mtName_;
        int tableRef_;
    };
}