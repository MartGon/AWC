#pragma once

#include <AWC.h>

#include <lua.hpp>

#include <Script/UserData/UserData.h>

namespace Script
{
    template<typename T>
    void Push(lua_State* state, T val);

    template<>
    void Push<int>(lua_State* state, int val);
    template<>
    void Push<std::string>(lua_State* state, std::string val);
    template<>
    void Push<const char*>(lua_State* state, const char* val);
    template<>
    void Push<bool>(lua_State* state, bool val);

    template <typename T>
    T To(lua_State* state, int index);
    template<>
    int To<int>(lua_State* state, int index);
    template<>
    std::string To<std::string>(lua_State* state, int index);
    template<>
    bool To<bool>(lua_State* state, int index);

    template <typename T>
    void GetField(lua_State*, int index, T key);
    template<>
    void GetField<int>(lua_State*, int index, int key);
    template<>
    void GetField<const char*>(lua_State*, int index, const char* key);
    template<>
    void GetField<std::string>(lua_State*, int index, std::string key);

    class LuaTable
    {
    public:
        // Creates a Wrapper for a table at the top of the stack
        LuaTable(lua_State* luaState, int index);

        // Creates a Wrapper for a new table
        LuaTable(lua_State* luaState);

        // Creates a Wrapper for a new table, with the given metatable.
        LuaTable(lua_State* luaState, std::string mtName);

        LuaTable(LuaTable&& lt);
        LuaTable(LuaTable& lt) = delete;

        ~LuaTable();
    
        template<typename T, typename K>
        void Set(K key, T val)
        {
            PushLuaTable();
            Push(luaState_, val);
            SetField(key);
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
        T* GetUserData(K key, const char* mtName)
        {
            PushLuaTable();
            GetField(luaState_, -1, key);
            T* ptr = Script::UserData::UserData::ToUserData<T>(luaState_, mtName, -1);
            lua_pop(luaState_, 2);

            return ptr;
        }

        template <typename T>
        typename T::type* SetGCData(std::string key, typename T::type userdata)
        {
            PushLuaTable();
            auto ptr = Script::UserData::UserData::PushGCData<T>(luaState_, userdata);
            SetField(key);

            return ptr;
        }

        template<typename T, typename K>
        T* SetGCData(K key, const char* mtName, T userdata)
        {
            PushLuaTable();
            auto ptr = Script::UserData::UserData::PushGCData(luaState_, mtName, userdata);
            SetField(key);

            return ptr;
        }

        template<typename T, typename K>
        void SetRawData(K key, const char* mtName, T userdata)
        {
            PushLuaTable();
            Script::UserData::UserData::PushRawData(luaState_, mtName, userdata);
            SetField(key);
        }

        std::unique_ptr<LuaTable> GetTable(std::string key);
        std::unique_ptr<LuaTable> GetTable(int index);

        void SetMetaTable(std::string mtName);
        int GetKeyType(std::string key);
        int GetIndexType(int index);
        bool ContainsValue(std::string key);
        int Length();

        void PushLuaTable();
        int GetRef()
        {
            return tableRef_;
        }

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