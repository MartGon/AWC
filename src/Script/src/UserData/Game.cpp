#include <Script/UserData/Game.h>

using namespace Script;

const char* MT_NAME = "AWC_Game";

void UserData::Game::Init(lua_State* luaState)
{
    const luaL_Reg methods[] = {
        {"GetMap", UserData::Game::GetMap},
        {"GetMapCount", UserData::Game::GetMapCount},
        {NULL, NULL}
    };

    luaL_newmetatable(luaState, MT_NAME);

    lua_pushstring(luaState, "__index");
    lua_pushvalue(luaState, -2);
    lua_settable(luaState, -3); // Metatable.__index = Metatable

    luaL_setfuncs(luaState, methods, 0);
    lua_pop(luaState, 1);

    return;
}

void UserData::Game::Push(lua_State* luaState, ::Game* game)
{
    lua_pushlightuserdata(luaState, game);
    luaL_getmetatable(luaState, MT_NAME);
    lua_setmetatable(luaState, -2);
}

int UserData::Game::GetMap(lua_State* L)
{
    int n = 0;

    return n;
}

int UserData::Game::GetMapCount(lua_State* luaState)
{
    ::Game* game = static_cast<::Game*>(luaL_checkudata(luaState, -1, MT_NAME));
    luaL_argcheck(luaState, game != nullptr, 1, "'Game' expected");

    int mapCount = game->GetMapCount();
    lua_pushinteger(luaState, mapCount);
    return 1;
}