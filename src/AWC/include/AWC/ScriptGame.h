#include <AWC/Game.h>

#include <lua.hpp>

class ScriptGame : public Game
{
public:
    ScriptGame()
    {
        luaState_ = luaL_newstate();
    }

    ~ScriptGame()
    {
        lua_close(luaState_);
    }

    lua_State* GetLuaState()
    {
        return luaState_;
    }

private:
    lua_State* luaState_;
};