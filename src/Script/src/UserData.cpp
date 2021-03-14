#include <Script/UserData.h>

using namespace Script;

void UserData::Init(lua_State* luaState)
{
    
    // Libs
    UserData::RegisterLib(luaState, Vector2::LIB_NAME, Vector2::functions);
    UserData::RegisterLib(luaState, TilePatternDesc::LIB_NAME, TilePatternDesc::functions);
    UserData::RegisterLib(luaState, MovementDescType::LIB_NAME, MovementDescType::functions);

    // Objects
    UserData::RegisterMetatable(luaState, Vector2::MT_NAME, Vector2::methods);
    UserData::RegisterMetatable(luaState, Game::MT_NAME, Game::methods);
    UserData::RegisterMetatable(luaState, Map::MT_NAME, Map::methods);
    UserData::RegisterMetatable(luaState, Unit::MT_NAME, Unit::methods);
    UserData::RegisterMetatable(luaState, UnitMovement::MT_NAME, UnitMovement::methods);
    UserData::RegisterMetatable(luaState, TilePatternDesc::MT_NAME, TilePatternDesc::methods);
    UserData::RegisterMetatable(luaState, MovementDescType::MT_NAME, MovementDescType::methods);
}