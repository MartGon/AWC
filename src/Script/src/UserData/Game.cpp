#include <Script/UserData/Game.h>
#include <Script/UserData/UserData.h>
#include <Script/UserData/Map.h>

using namespace Script;

const char* UserData::Game::MT_NAME = "AWC_Game";
const luaL_Reg UserData::Game::methods[] = {
        {"GetMap", Game::GetMap},
        {"GetMapCount", Game::GetMapCount},
        {NULL, NULL}
    };

int UserData::Game::GetMap(lua_State* L)
{   
    auto game = UserData::ToUserData<Game>(L, 1);
    auto index = luaL_checkinteger(L, 2);

    bool indexValid = index < game->GetMapCount() && index >= 0;
    
    if(indexValid)
    {
        auto& map = game->GetMap(index);
        UserData::PushDataRef<Map>(L, &map);
    }
    else
        luaL_error(L, "Map index %d is not valid", index);

    return 1;
}

int UserData::Game::GetMapCount(lua_State* luaState)
{
    auto game = UserData::ToUserData<Game>(luaState, 1);

    int mapCount = game->GetMapCount();
    lua_pushinteger(luaState, mapCount);
    return 1;
}