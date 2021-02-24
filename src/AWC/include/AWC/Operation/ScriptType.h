#pragma once

#include <string>

#include <lua.hpp>

#include <AWC/Operation/Script.h>

namespace Operation
{
    class ScriptType
    {
    public:
        ScriptType(lua_State* state, std::string scriptPath);

        std::shared_ptr<Script> CreateScript();

        Result Execute(Game& game, uint8_t prio, int tableRef);

    private:
        std::string scriptPath_;

        lua_State* luaState_;
        int executeRef_;
        int undoRef_;
    };
}