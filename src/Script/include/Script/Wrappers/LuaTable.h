#pragma once

#include <AWC.h>

#include <lua.hpp>

#include <Script/UserData/UserData.h>

#include <Script/Wrappers/LuaFunction.h>

namespace Script
{
    template <Scope s = Scope::Internal>
    class LuaTable
    {
    public:
        // Creates a Wrapper for a table at the given index of the stack
        LuaTable(lua_State* luaState, int index) : luaState_{luaState}
        {
            CheckExpectedArg<s>(luaState, IsTable(luaState, index), index, "LuaTable");
            lua_pushvalue(luaState, index);
            tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        }

        // Creates a Wrapper for a table located at tIndex of a table at sIndex of the Stack
        template <typename K>
        LuaTable(lua_State* luaState, int sIndex, K tIndex) : luaState_{luaState}
        {
            CheckExpectedArg<s>(luaState, IsTable(luaState, sIndex), sIndex, "LuaTable");
            int top = lua_gettop(luaState);
            GetField<K>(luaState, sIndex, tIndex);
            CheckExpectedArgInTable<s>(luaState, IsTable(luaState, -1), sIndex, tIndex, "LuaTable",
                [top](lua_State* luaState){
                    lua_settop(luaState, top);
                });
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

        // Copy operations
        LuaTable(const LuaTable& lt) = delete;
        LuaTable& operator=(const LuaTable&) = delete;     

        // Move Operations
        LuaTable(LuaTable&& other) : tableRef_{other.tableRef_}, luaState_{other.luaState_}, mtName_{other.mtName_}
        {
            other.tableRef_ = LUA_REFNIL;
        }

        LuaTable& operator=(LuaTable&& other)
        {
            luaL_unref(luaState_, LUA_REGISTRYINDEX, tableRef_);

            tableRef_ = other.tableRef_;
            luaState_ = other.luaState_;
            mtName_ = std::move(other.mtName_);

            other.tableRef_ = LUA_REFNIL;

            return *this;
        }
        
        // Destructor
        ~LuaTable()
        {
            luaL_unref(luaState_, LUA_REGISTRYINDEX, tableRef_);   
        }

        // Getters and Setters
        template<typename T, typename K>
        void Set(K key, T val)
        {
            PushInternal();
            Push(luaState_, val);
            SetField(key);
            lua_pop(luaState_, 1);
        }

        template<typename T, typename K>
        T Get(K key)
        {
            PushInternal();
            GetField(luaState_, -1, key);
            auto val = To<T>(luaState_, -1);
            lua_pop(luaState_, 2);

            return val;
        }

        template <typename T, typename K>
        typename T::type* GetUserData(K key)
        {
            PushInternal();
            auto type = GetField(luaState_, -1, key);
            auto ptr = Script::UserData::UserData::CastOrCreate<T, s>(luaState_, -1);
            lua_pop(luaState_, 2); // Pop Table and field

            return ptr;
        }

        template <typename T, typename K>
        typename T::type* SetDataCopy(K key, typename T::type userdata)
        {
            PushInternal();
            auto ptr = Script::UserData::UserData::PushDataCopy<T>(luaState_, userdata);
            SetField(key);
            lua_pop(luaState_, 1);

            return ptr;
        }

        template<typename T, typename K>
        void SetDataRef(K key, typename T::type* userdata)
        {
            PushInternal();
            Script::UserData::UserData::PushDataRef<T>(luaState_, userdata);
            SetField(key);
            lua_pop(luaState_, 1);
        }

        template<typename T, typename K>
        std::optional<T> GetLuaWrapper(K key)
        {
            PushInternal();
            int type = GetField(luaState_, -1, key);

            std::optional<T> lt{std::in_place, luaState_, -1};

            lua_pop(luaState_, 2);

            return lt;
        }

        template <typename T, typename K>
        void SetLuaWrapper(K key, T& val)
        {
            PushInternal();
            val.PushInternal();
            SetField(key); 
            lua_pop(luaState_, 1);
        }

        template<typename K>
        int GetType(K key)
        {
            PushInternal();
            int type = GetField(luaState_, -1, key);
            lua_pop(luaState_, 2);

            return type;
        }

        template <typename K>
        bool ContainsValue(K key)
        {            
            return GetType(key) != LUA_TNIL;
        }

        int Length()
        {
            PushInternal();
            int len = lua_rawlen(luaState_, -1);
            lua_pop(luaState_, 1);

            return len;
        }

        void SetMetaTable(std::string mtName)
        {
            PushInternal();
            luaL_setmetatable(luaState_, mtName.c_str());
            lua_pop(luaState_, 1);
        }

        std::string GetMetaTableName()
        {
            std::string name;

            PushInternal();
            if(lua_getmetatable(luaState_, -1))
            {
                lua_getfield(luaState_, -1, "__name");
                name = std::string(lua_tostring(luaState_, -1));
                lua_pop(luaState_, 2); // Pops metatable and __name value
            }
            lua_pop(luaState_, 1);

            return name;
        }

        void PushInternal()
        {
            lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
        }
    private:

        // Assumes that table is at -2 and value is at the top of the stack
        // Pops the internal lua table from the stack
        void SetField(std::string key)
        {
            lua_setfield(luaState_, -2, key.c_str());
        }
        void SetField(int index)
        {
            lua_seti(luaState_, -2, index);
        }

        lua_State* luaState_;
        std::string mtName_;
        int tableRef_;
    };
}