#pragma once

#include <AWC/Operation/OperationI.h>

#include <Script/UserData.h>
#include <Script/LuaTable.h>

#include <lua.hpp>

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
        ScriptOperation(lua_State* luaState, Type& type) : 
            scriptType_{type}, luaState_{luaState}, argsTable_{luaState, "_MAIN"}, OperationI{SCRIPT}
        {

        }

        lua_State* luaState_;
        LuaTable argsTable_;
        Type& scriptType_;
    };
}