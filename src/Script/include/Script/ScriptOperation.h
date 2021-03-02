#pragma once

#include <AWC/Operation/OperationI.h>

#include <Script/UserData.h>

#include <lua.hpp>

namespace Script
{
    class ScriptOperation;
}

class LuaTable
{
friend class Script::ScriptOperation;
public:
    LuaTable(lua_State* luaState) : luaState_{luaState}
    {
        lua_newtable(luaState);
        tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
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

private:

    int GetRef()
    {
        return tableRef_;
    }

    lua_State* luaState_;
    int tableRef_;
};

namespace Script
{
    class Type;

    const unsigned int SCRIPT = 9;

    class ScriptOperation : public Operation::OperationI
    {
    friend class Type;

    public:
        ~ScriptOperation() override
        {

        }

        LuaTable& GetArgsTable()
        {
            return argsTable_;
        }

        Operation::Result Execute(::Game& state, uint8_t prio) override;

    private:
        ScriptOperation(lua_State* luaState, Type& type) : scriptType_{type}, luaState_{luaState}, argsTable_{luaState}, OperationI{SCRIPT}
        {
            
        }

        lua_State* luaState_;
        LuaTable argsTable_;
        Type& scriptType_;
    };
}