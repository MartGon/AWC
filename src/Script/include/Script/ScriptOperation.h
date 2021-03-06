#pragma once

#include <AWC/Operation/OperationI.h>

#include <Script/UserData.h>
#include <Script/LuaTable.h>

#include <lua.hpp>

namespace Script
{
    class Type;

    extern const unsigned int SCRIPT;
    extern const char* MT_NAME;

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
        ScriptOperation(lua_State* luaState, Type& type) : 
            scriptType_{type}, luaState_{luaState}, argsTable_{luaState, MT_NAME}, OperationI{SCRIPT}
        {

        }

        lua_State* luaState_;
        LuaTable argsTable_;
        Type& scriptType_;
    };
}