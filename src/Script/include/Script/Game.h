#pragma once

#include <lua.hpp>

#include <AWC/Game.h>

#include <Script/Wrappers/LuaVM.h>
#include <Script/ScriptType.h>
#include <Script/UserData.h>
#include <Script/Wrappers/LuaTable.h>
#include <Script/ScriptOperation.h>

#include <Script/DB/Database.h>

namespace Script
{
    class Game
    {
    public:
        using Database = DB::Database<UnitType, Script::Type, std::shared_ptr<Script::ScriptOperation>, 
                                    TileType>;

        Game()
        {
            InitState();
        }

        ~Game()
        {
        }

        ::Game& GetGame()
        {
            return game_;
        }
        
        LuaVM& GetLuaVM()
        {
            return vm_;
        }

        lua_State* GetLuaState()
        {
            return vm_.GetLuaState();
        }

        Database& GetDB()
        {
            return db;
        }

        // Scripts
        unsigned int CreateScriptType(std::string scriptPath);
        unsigned int CreateScript(unsigned int typeId);
        LuaTable<Scope::External>& GetScriptTable(unsigned int id);
        unsigned int PushScript(unsigned int id, unsigned int prio = PRIORITY_DEFAULT);

        // Config
        void RunConfig(std::string configPath);

    private:

        void InitState();

        LuaVM vm_;
        Database db;
        ::Game game_;
    };

}