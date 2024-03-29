#pragma once

#include <AWC/Operation/OperationI.h>

#include <Script/UserData.h>
#include <Script/Wrappers/LuaTable.h>

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

        LuaTable<Scope::External>& GetArgsTable()
        {
            return argsTable_;
        }

        Operation::Result Execute(::Game& state, const Process::Process& p) override;
        void Undo(::Game& state, const Process::Process& p) override;

    private:
        ScriptOperation(lua_State* luaState, const Type& type) : 
            scriptType_{type}, luaState_{luaState}, argsTable_{luaState, MT_NAME}, OperationI{SCRIPT}
        {

        }

        lua_State* luaState_;
        LuaTable<Scope::External> argsTable_;
        const Type& scriptType_;
    };
}