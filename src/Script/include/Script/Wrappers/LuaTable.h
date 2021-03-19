#pragma once

#include <AWC.h>

#include <lua.hpp>

#include <Script/UserData/UserData.h>
#include <Script/Utils/Utils.h>

namespace Script
{
    class LuaTable
    {
    public:
        // Creates a Wrapper for a table at the given index of the stack
        LuaTable(lua_State* luaState, int index);

        // Creates a Wrapper for a table located at tIndex of a table at sIndex of the Stack
        template <typename K>
        LuaTable(lua_State* luaState, int sIndex, K tIndex) : luaState_{luaState}
        {
            GetField<K>(luaState, sIndex, tIndex);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Creates a Wrapper for a new table
        LuaTable(lua_State* luaState);

        // Creates a Wrapper for a new table, with the given metatable.
        LuaTable(lua_State* luaState, std::string mtName);

        LuaTable(LuaTable&& lt);
        LuaTable(const LuaTable& lt) = delete;

        ~LuaTable();
    
        template<typename T, typename K>
        void Set(K key, T val)
        {
            PushLuaTable();
            Push(luaState_, val);
            SetField(key);
            lua_pop(luaState_, 1);
        }

        template<typename T, typename K>
        T Get(K key)
        {
            PushLuaTable();
            GetField(luaState_, -1, key);
            auto val = To<T>(luaState_, -1);
            lua_pop(luaState_, 2);

            return val;
        }

        template <typename T, typename K>
        typename T::type* GetUserData(K key)
        {
            PushLuaTable();
            GetField(luaState_, -1, key);
            auto ptr = Script::UserData::UserData::ToUserData<T>(luaState_, -1);
            lua_pop(luaState_, 2);

            return ptr;
        }

        template <typename T, typename K>
        typename T::type* SetDataCopy(K key, typename T::type userdata)
        {
            PushLuaTable();
            auto ptr = Script::UserData::UserData::PushDataCopy<T>(luaState_, userdata);
            SetField(key);
            lua_pop(luaState_, 1);

            return ptr;
        }

        template<typename T, typename K>
        void SetDataRef(K key, typename T::type* userdata)
        {
            PushLuaTable();
            Script::UserData::UserData::PushDataRef<T>(luaState_, userdata);
            SetField(key);
            lua_pop(luaState_, 1);
        }

        template<typename K>
        std::unique_ptr<LuaTable> GetTable(K key)
        {
            PushLuaTable();
            int type = GetField(luaState_, -1, key);

            std::unique_ptr<LuaTable> ptr;
            if(type == LUA_TTABLE)
                ptr = std::unique_ptr<LuaTable>{new LuaTable{luaState_, -1}};

            lua_pop(luaState_, 2);

            return ptr;
        }

        template <typename K>
        void SetTable(K key, LuaTable& table)
        {
            PushLuaTable();
            table.PushLuaTable();
            SetField(key); 
            lua_pop(luaState_, 1);
        }

        template<typename K>
        int GetType(K key)
        {
            PushLuaTable();
            int type = GetField(luaState_, -1, key);
            lua_pop(luaState_, 2);

            return type;
        }

        template <typename K>
        bool ContainsValue(K key)
        {            
            return GetType(key) != LUA_TNIL;
        }

        int Length();
        void SetMetaTable(std::string mtName);
        std::string GetMetaTableName();

        void PushLuaTable();
    private:

        // Assumes that table is at -2 and value is at the top of the stack
        // Pops the internal lua table from the stack
        void SetField(std::string key);
        void SetField(int index);

        lua_State* luaState_;
        std::string mtName_;
        int tableRef_;
    };
}