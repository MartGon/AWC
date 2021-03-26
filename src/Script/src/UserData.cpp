#include <Script/UserData.h>
#include <Script/UserData/Database.h>

using namespace Script;

template <typename T>
void Init(lua_State* luaState)
{
    UserData::UserData::RegisterLib<T>(luaState);
    UserData::UserData::RegisterMetatable<T>(luaState);
}

template <typename T, typename Y, typename ...Ts>
void Init(lua_State* luaState)
{
    Init<T>(luaState);
    Init<Y, Ts...>(luaState);
}

void UserData::Init(lua_State* luaState)
{
    ::Init<Vector2, 
        AreaDesc,
        Database,
        MovementDescType,
        Game,
        Map,
        Unit,
        UnitMovement,
        CostTable,
        Player,
        UnitType
        >(luaState);
}