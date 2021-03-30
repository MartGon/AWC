#include <Script/UserData.h>

using namespace Script;

const char* UserData::UnitAttack::MT_NAME = "AWC_UnitAttack";
const char* UserData::UnitAttack::LIB_NAME = "UnitAttack";
const luaL_Reg UserData::UnitAttack::methods[] = {
    {"CanAttack", UnitAttack::CanAttack},
    {NULL, NULL}
};
const luaL_Reg UserData::UnitAttack::functions[] = {
    {NULL, NULL}
};

int UserData::UnitAttack::CanAttack(lua_State* luaState)
{
    auto unitAttack = UserData::CheckUserData<UnitAttack>(luaState, 1);
    auto dest = *UserData::CheckUserData<Vector2>(luaState, 2);

    bool result = unitAttack->CanAttack(dest);
    lua_pushboolean(luaState, result);

    return 1;
}