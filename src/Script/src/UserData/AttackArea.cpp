#include <Script/UserData.h>

using namespace Script;

const char* UserData::AttackArea::MT_NAME = "AWC_AttackArea";
const char* UserData::AttackArea::LIB_NAME = "AttackArea";
const luaL_Reg UserData::AttackArea::methods[] = {
    {"CanAttack", AttackArea::CanAttack},
    {"GetTilesInRangeCount", AttackArea::GetTilesInRangeCount},
    {"GetTileInRange", AttackArea::GetTileInRange},
    {NULL, NULL}
};
const luaL_Reg UserData::AttackArea::functions[] = {
    {NULL, NULL}
};

int UserData::AttackArea::CanAttack(lua_State* luaState)
{
    auto unitAttack = UserData::CheckUserData<AttackArea>(luaState, 1);
    auto dest = *UserData::CheckUserData<Vector2>(luaState, 2);

    bool result = unitAttack->CanAttack(dest);
    lua_pushboolean(luaState, result);

    return 1;
}

int UserData::AttackArea::GetTilesInRangeCount(lua_State* luaState)
{
    auto unitAttack = UserData::CheckUserData<AttackArea>(luaState, 1);
    auto count = unitAttack->GetTilesInRange().size();

    lua_pushinteger(luaState, count);

    return 1;
}

int UserData::AttackArea::GetTileInRange(lua_State* luaState)
{
    auto unitAttack = UserData::CheckUserData<AttackArea>(luaState, 1);
    auto index = luaL_checkinteger(luaState, 2) - 1;

    auto tilesPos = unitAttack->GetTilesInRange();
    auto count = tilesPos.size();
    bool validIndex = index < count && index >= 0;
    CheckArg<Scope::Internal>(luaState, validIndex, 2, "Pos index " + std::to_string(index) + " is not valid");

    UserData::PushDataCopy<Vector2>(luaState, tilesPos[index]);

    return 1;
}