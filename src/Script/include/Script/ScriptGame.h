#pragma once

#include <lua.hpp>

#include <AWC/Game.h>

#include <Script/ScriptType.h>
#include <Script/UserData.h>

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
        void PushScript(unsigned int id, unsigned int prio = PRIORITY_DEFAULT);

    private:

        void InitState();

        LuaState ls;

        std::unordered_map<unsigned int, Script::Type> scriptTypes_;
        unsigned int stIndex_ = 0;

        std::unordered_map<unsigned int, std::shared_ptr<Script::ScriptOperation>> scripts_;
        unsigned int sIndex_ = 0;

        ::Game game_;
    };

}