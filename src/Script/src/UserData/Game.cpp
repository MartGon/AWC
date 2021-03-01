#include <Script/UserData/Game.h>
#include <Script/UserData/UserData.h>

using namespace Script;

const char* MT_NAME = "AWC_Game";

void UserData::Game::Init(lua_State* luaState)
{
    const luaL_Reg methods[] = {
        {"GetMap", Game::GetMap},
        {"GetMapCount", Game::GetMapCount},
        {NULL, NULL}
    };

    UserData::RegisterMetatable(luaState, MT_NAME, methods);

    return;
}

void UserData::Game::Push(lua_State* luaState, ::Game* game)
{
    UserData::Push(luaState, MT_NAME, game);
}

int UserData::Game::GetMap(lua_State* L)
{
    int n = 0;

    return n;
}

int UserData::Game::GetMapCount(lua_State* luaState)
{
    auto game = UserData::ToUserData<::Game>(luaState, MT_NAME);

    int mapCount = game->GetMapCount();
    lua_pushinteger(luaState, mapCount);
    return 1;
}