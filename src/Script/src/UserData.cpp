#include <Script/UserData.h>

using namespace Script;

void UserData::Init(lua_State* luaState)
{
    // Special case
    Vector2::Init(luaState);

    // Objects
    UserData::RegisterMetatable(luaState, Game::MT_NAME, Game::methods);
    UserData::RegisterMetatable(luaState, Map::MT_NAME, Map::methods);
    UserData::RegisterMetatable(luaState, Unit::MT_NAME, Unit::methods);
    UserData::RegisterMetatable(luaState, UnitMovement::MT_NAME, UnitMovement::methods);
}