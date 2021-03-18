#include <Script/UserData/UnitMovement.h>

#include <Script/UserData/UserData.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::UnitMovement::MT_NAME = "AWC_UnitMovement";
const char* UserData::UnitMovement::LIB_NAME = "UnitMovement";
const luaL_Reg UserData::UnitMovement::methods[] = {
    {"CanMove", UnitMovement::CanMove},
    {NULL, NULL}
};
const luaL_Reg UserData::UnitMovement::functions[] = {
    {NULL, NULL}
};

int UserData::UnitMovement::CanMove(lua_State* luaState)
{
    auto unitMove = UserData::ToUserData<UnitMovement>(luaState, 1);
    auto dest = UserData::ToUserData<Vector2>(luaState, 2);

    bool result = unitMove->CanMove(*dest);
    lua_pushboolean(luaState, result);

    return 1;
}