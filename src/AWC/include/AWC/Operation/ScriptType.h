#pragma once

#include <string>

#include <lua.hpp>

namespace Operation
{
    class ScriptType
    {
    public:
        ScriptType(lua_State* state, std::string scriptPath);

    private:
        std::string scriptPath_;

        int executeRef_;
        int undoRef_;
    };
}