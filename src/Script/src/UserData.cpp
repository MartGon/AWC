#include <Script/UserData.h>

using namespace Script;

void UserData::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, Vector2::MT_NAME, Vector2::methods);
    UserData::RegisterLib(luaState, Vector2::LIB_NAME, Vector2::functions);

    UserData::RegisterMetatable(luaState, Game::MT_NAME, Game::methods);
    UserData::RegisterMetatable(luaState, Map::MT_NAME, Map::methods);
    UserData::RegisterMetatable(luaState, Unit::MT_NAME, Unit::methods);
    UserData::RegisterMetatable(luaState, UnitMovement::MT_NAME, UnitMovement::methods);
}