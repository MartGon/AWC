#pragma once

#include <lua.hpp>

#include <AWC/Game.h>

#include <Script/ScriptType.h>
#include <Script/UserData.h>

#include <Script/DB/Database.h>

namespace Script
{
    class LuaState
    {
    public:
        LuaState() : L{luaL_newstate()}
        {

        }

        ~LuaState()
        {
            lua_close(L);
        }

        lua_State* GetLuaState()
        {
            return L;
        }

    private:
        lua_State* L;
    };

    class Game
    {
    public:
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
        
        lua_State* GetLuaState()
        {
            return ls.GetLuaState();
        }

        unsigned int CreateScriptType(std::string scriptPath);
        unsigned int CreateScript(unsigned int typeId);
        LuaTable& GetScriptTable(unsigned int id);
        unsigned int PushScript(unsigned int id, unsigned int prio = PRIORITY_DEFAULT);

    private:

        void InitState();

        LuaState ls;

        DB::Database<UnitType, Script::Type, std::shared_ptr<Script::ScriptOperation>> db;
        ::Game game_;
    };

}