#pragma once

#include <lua.hpp>

#include <string>
#include <filesystem>

#include <Script/Wrappers/LuaState.h>
#include <Script/Wrappers/LuaTable.h>

namespace Script
{
    class LuaVM
    {
    public:
        LuaVM() : global_{ls_.L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS}
        {

        }
        
        LuaVM(const LuaVM&) = delete;

        lua_State* GetLuaState()
        {
            return ls_.L;
        }

        LuaTable<Scope::External>& GetGlobalTable()
        {
            return global_;
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

        // Modules
        void AppendToPath(std::filesystem::path path);

    private:
        LuaState ls_;
        LuaTable<Scope::External> global_;
    };
}