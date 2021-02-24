#pragma once

#include <AWC/Operation/OperationI.h>

#include <lua.hpp>

namespace Operation
{
    class Script;
}

class LuaTable
{
friend class Operation::Script;
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

private:

    int GetRef()
    {
        return tableRef_;
    }

    lua_State* luaState_;
    int tableRef_;
};

namespace Operation
{
    class ScriptType;

    class Script : public OperationI
    {
    friend class ScriptType;

    public:
        ~Script() override
        {

        }

        LuaTable& GetArgsTable()
        {
            return argsTable_;
        }

        Result Execute(Game& state, uint8_t prio) override;

    private:
        Script(lua_State* luaState, ScriptType& type) : scriptType_{type}, luaState_{luaState}, argsTable_{luaState}, OperationI{0}
        {
            
        }

        lua_State* luaState_;
        LuaTable argsTable_;
        ScriptType& scriptType_;
    };
}