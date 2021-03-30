#include <Script/UserData/Game.h>
#include <Script/UserData/UserData.h>

#include <Script/UserData.h>

using namespace Script;

const char* UserData::Game::MT_NAME = "AWC_Game";
const char* UserData::Game::LIB_NAME = "Game";
const luaL_Reg UserData::Game::methods[] = {
        {"CreateMap", Game::CreateMap},
        {"GetMap", Game::GetMap},
        {"GetMapCount", Game::GetMapCount},
        {"CreatePlayer", Game::CreatePlayer},
        {NULL, NULL}
    };
const luaL_Reg UserData::Game::functions[] = {
    {NULL, NULL}
};

int UserData::Game::CreateMap(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    ::Vector2 size;
    if(IsType(L, 2, LUA_TNUMBER))
    {
        size.x = luaL_checkinteger(L, 2);
        size.y = luaL_checkinteger(L, 3);
    }
    else
    {
        size = *UserData::CheckUserData<Vector2>(L, 2);
    }
    auto mapId = game->CreateMap(size.x, size.y);
    UserData::PushDataRef<Map>(L, &game->GetMap(mapId));

    return 1;
}

int UserData::Game::GetMap(lua_State* L)
{   
    auto game = UserData::CheckUserData<Game>(L, 1);
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
    auto game = UserData::CheckUserData<Game>(luaState, 1);

    int mapCount = game->GetMapCount();
    lua_pushinteger(luaState, mapCount);
    return 1;
}

int UserData::Game::CreatePlayer(lua_State* luaState)
{
    auto game = UserData::CheckUserData<Game>(luaState, 1);
    auto teamId = luaL_checkinteger(luaState, 2);

    auto id = game->AddPlayer(teamId, 0);
    auto p = game->GetPlayer(id);
    UserData::PushDataRef<Player>(luaState, p.get());

    return 1;
}