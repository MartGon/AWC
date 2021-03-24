#pragma once

#include <lua.hpp>

#include <string>

#include <Script/Wrappers/LuaState.h>
#include <Script/Wrappers/LuaTable.h>

namespace Script
{
    class LuaVM
    {
    public:
        LuaVM() : global{ls.L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS}
        {

        }
        
        LuaVM(const LuaVM&) = delete;

        lua_State* GetLuaState()
        {
            return ls.L;
        }

        LuaTable<Scope::External>& GetGlobalTable()
        {
            return global;
        }

        // Basic lib
        int GetTop();
        void Pop(int amount);
        unsigned int Call(int nargs, int nresults, int msgh);

        // Aux Lib

        // Loads a lua chunk into a file
        unsigned int LoadFile(std::string path);
        unsigned int RunFile(std::string path);
        unsigned int RunFile(std::string path, LuaTable<Scope::External>& env);

    private:
        LuaState ls;
        LuaTable<Scope::External> global;
    };
}