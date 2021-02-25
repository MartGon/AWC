#pragma once

#include <lua.hpp>

#include <AWC/Game.h>
#include <AWC/Operation/ScriptType.h>

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

class ScriptGame
{
public:
    ScriptGame()
    {
    }

    ~ScriptGame()
    {
    }

    Game& GetGame()
    {
        return game_;
    }

#ifdef _DEBUG

    lua_State* GetLuaState()
    {
        return ls.GetLuaState();
    }

#endif

    unsigned int CreateScriptType(std::string scriptPath);
    unsigned int CreateScript(unsigned int typeId);
    LuaTable& GetScriptTable(unsigned int id);
    void PushScript(unsigned int id, unsigned int prio = PRIORITY_DEFAULT);

private:

    LuaState ls;

    std::unordered_map<unsigned int, Operation::ScriptType> scriptTypes_;
    unsigned int stIndex_ = 0;

    std::unordered_map<unsigned int, std::shared_ptr<Operation::Script>> scripts_;
    unsigned int sIndex_ = 0;

    Game game_;
    lua_State* luaState_;
};