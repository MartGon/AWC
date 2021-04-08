#include <Script/UserData.h>

using namespace Script;

const char* UserData::Unit::MT_NAME = "AWC_Unit";
const char* UserData::Unit::LIB_NAME = "Unit";
const luaL_Reg UserData::Unit::methods[] = {
    {"CalculateMovement", Unit::CalculateMovement},
    {"CalculateAttack", Unit::CalculateAttack},
    {"GetOwner", Unit::GetOwner},
    {"GetGUID", Unit::GetGUID},
    {NULL, NULL}
};
const luaL_Reg UserData::Unit::functions[] = {
    {NULL, NULL}
};

int UserData::Unit::CalculateMovement(lua_State* luaState)
{
    auto unit = *UserData::CheckUserData<Unit>(luaState, 1);
    auto map = UserData::CheckUserData<Map>(luaState, 2);
    auto vector2 = UserData::CheckUserData<Vector2>(luaState, 3);

    auto unitMovement = unit->CalculateMovement(*map, *vector2);
    auto unitMove = UserData::PushDataCopy<MoveArea>(luaState, unitMovement);

    return 1;
}

int UserData::Unit::CalculateAttack(lua_State* luaState)
{
    auto unit = *UserData::CheckUserData<Unit>(luaState, 1);
    auto weaponId = lua_tointeger(luaState, 2);
    auto map = UserData::CheckUserData<Map>(luaState, 3);
    auto vector2 = UserData::CheckUserData<Vector2>(luaState, 4);

    auto unitAttack = unit->CalculateAttack(weaponId, *map, *vector2);
    auto attack = UserData::PushDataCopy<AttackArea>(luaState, unitAttack);

    return 1;
}

int UserData::Unit::GetOwner(lua_State* luaState)
{
    auto unit = *UserData::CheckUserData<Unit>(luaState, 1);
    auto player = unit->GetOwner();

    UserData::PushDataRef<Player>(luaState, player);

    return 1;
}

int UserData::Unit::GetGUID(lua_State* luaState)
{
    auto unit = *UserData::CheckUserData<Unit>(luaState, 1);

    UserData::PushDataCopy<EntityGUID>(luaState, unit->GetGUID());

    return 1;
}