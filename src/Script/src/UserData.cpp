#include <Script/UserData.h>
#include <Script/UserData/Database.h>

using namespace Script;

template<typename T>
auto InitLibImp(lua_State* luaState, int) -> decltype(T::InitLib(luaState))
{
    return T::InitLib(luaState);
}

template<typename T>
auto InitLibImp(lua_State* luaState, double) -> int
{
    return 0;
}

template<typename T>
int InitLib(lua_State* luaState)
{
    return InitLibImp<T>(luaState, 0);
}

template <typename T>
void Init(lua_State* luaState)
{
    UserData::UserData::RegisterLib<T>(luaState);
    UserData::UserData::RegisterMetatable<T>(luaState);
    InitLib<T>(luaState);
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
        UnitType,
        TileType,
        Tile,
        Process,
        Trigger,
        WeaponType,
        UnitAttack,
        EventHandler,
        EventNotification,
        EntityGUID,
        Operation
        >(luaState);
}