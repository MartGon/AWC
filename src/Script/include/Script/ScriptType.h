#pragma once

#include <string>
#include <array>

#include <lua.hpp>

#include <Script/ScriptOperation.h>

#include <Script/Wrappers/LuaFunction.h>

namespace Script
{
    class Game;
    class LuaVM;

    class Type
    {
    friend class Game;

    public:
        Type(LuaVM& vm, std::string scriptPath);
        Type(const Type&) = delete;
        Type(Type&&) = default;

        std::shared_ptr<ScriptOperation> CreateScript() const;

        Operation::Result Execute(::Game& game, uint8_t prio, LuaTable& tableRef) const;

    private:
        std::string scriptPath_;
        LuaVM& vm_;
        
        LuaFunction execute_;
        LuaFunction undo_;
    };
}