#pragma once

#include <string>

#include <lua.hpp>

#include <AWC/Operation/Script.h>

class ScriptGame;

namespace Operation
{
    class ScriptType
    {
    friend class ::ScriptGame;

    public:
        std::shared_ptr<Script> CreateScript();

        Result Execute(Game& game, uint8_t prio, int tableRef);

    private:
        ScriptType(lua_State* state, std::string scriptPath);

        std::string scriptPath_;

        lua_State* luaState_;
        int executeRef_;
        int undoRef_;
    };
}