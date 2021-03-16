#pragma once

#include <string>

#include <lua.hpp>

#include <Script/ScriptOperation.h>

namespace Script
{
    class Game;

    class Type
    {
    friend class Game;

    public:
        std::shared_ptr<ScriptOperation> CreateScript() const;

        Operation::Result Execute(::Game& game, uint8_t prio, LuaTable& tableRef) const;

    private:
        Type(lua_State* state, std::string scriptPath);

        std::string scriptPath_;

        lua_State* luaState_;
        int executeRef_;
        int undoRef_;
    };
}