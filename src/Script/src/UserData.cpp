#include <Script/UserData.h>
#include <Script/UserData/Database.h>

using namespace Script;

void UserData::Init(lua_State* luaState)
{
    // Libs
    UserData::RegisterLib<Vector2>(luaState);
    UserData::RegisterLib<AreaDesc>(luaState);
    UserData::RegisterLib<MovementDescType>(luaState);
    UserData::RegisterLib<Database>(luaState);

    // Objects
    UserData::RegisterMetatable<Vector2>(luaState);
    UserData::RegisterMetatable<Game>(luaState);
    UserData::RegisterMetatable<Map>(luaState);
    UserData::RegisterMetatable<Unit>(luaState);
    UserData::RegisterMetatable<UnitMovement>(luaState);
    UserData::RegisterMetatable<AreaDesc>(luaState);
    UserData::RegisterMetatable<MovementDescType>(luaState);
    UserData::RegisterMetatable<Database>(luaState);
}