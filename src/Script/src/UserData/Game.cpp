#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::Game::MT_NAME = "AWC_Game";
const char* UserData::Game::LIB_NAME = "Game";
const luaL_Reg UserData::Game::methods[] = {
        {"CreateMap", Game::CreateMap},
        {"GetMap", Game::GetMap},
        {"GetMapCount", Game::GetMapCount},

        {"GetUnit", Game::GetUnit},
        {"GetUnitPos", Game::GetUnitPos},

        {"CreatePlayer", Game::CreatePlayer},
        {"RemovePlayer", Game::RemovePlayer},

        {"CancelProcess", Game::CancelProcess},
        {"GetHistoryCount", Game::GetHistoryCount},
        {"GetHistoryProcess", Game::GetHistoryProcess},

        {"AddEventHandler", Game::AddEventHandler},
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

int UserData::Game::GetUnit(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    auto guid = UserData::CheckUserData<EntityGUID>(L, 2);
    auto unit = game->GetUnit(*guid);

    if(unit)
        UserData::PushDataCopy<Unit>(L, unit);
    else
        lua_pushnil(L);

    return 1;
}

int UserData::Game::GetUnitPos(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    auto guid = UserData::CheckUserData<EntityGUID>(L, 2);
    auto pos = game->GetUnitPos(*guid);
    // TODO - Create a way to auto unregister unit on remove
    //CheckArg<Scope::Internal>(L, pos.has_value(), 2, "Unit position could not be found!!!");
    if(pos.has_value())
    {
        LuaTable<Scope::Internal> position{L};
        position.SetDataCopy<Vector2>("pos", pos->pos);
        position.Set("mapId", pos->mapIndex);
        position.PushInternal();
    }
    else
        lua_pushnil(L);

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

int UserData::Game::RemovePlayer(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    auto playerId = luaL_checkinteger(L, 2);

    auto playerCount = game->GetPlayerCount();
    bool validIndex = playerId < playerCount && playerId >= 0;
    CheckArg<Scope::Internal>(L, validIndex, 2, "Player index " + std::to_string(playerId) + " is not valid");

    game->RemovePlayer(playerId);

    return 0;
}

int UserData::Game::CancelProcess(lua_State* luaState)
{
    auto game = UserData::CheckUserData<Game>(luaState, 1);
    auto p = UserData::CheckUserData<Process>(luaState, 2);

    ::Process::Trigger::Trigger trigger{::Process::Trigger::Type::OPERATION, p->id};
    OperationIPtr antiOp = std::make_shared<::Operation::AntiOperation>(p->id);
    game->Push(antiOp, trigger, p->priority + 1);

    return 0;
}

int UserData::Game::GetHistoryCount(lua_State* luaState)
{
    auto game = UserData::CheckUserData<Game>(luaState, 1);

    lua_pushinteger(luaState, game->GetHistoryCount());

    return 1;
}

int UserData::Game::GetHistoryProcess(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    auto index = luaL_checkinteger(L, 2) - 1;
    
    auto count = game->GetHistoryCount();
    bool validIndex = index < count && index >= 0;
    CheckArg<Scope::Internal>(L, validIndex, 2, "History index " + std::to_string(index) + " is not valid");

    auto p = game->GetHistoryProcess(index);
    UserData::PushAsTable<Process>(L, *p);

    return 1;
}

int UserData::Game::AddEventHandler(lua_State* L)
{
    auto game = UserData::CheckUserData<Game>(L, 1);
    auto eventHandler = *UserData::CheckUserData<EventHandler>(L, 2);

    auto& subject = game->GetSubject();
    subject.Register(eventHandler);

    return 0;
}