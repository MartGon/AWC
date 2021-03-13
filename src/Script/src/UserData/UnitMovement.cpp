#include <Script/UserData/UnitMovement.h>

#include <Script/UserData/UserData.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::UnitMovement::MT_NAME = "AWC_UnitMovement";
const luaL_Reg UserData::UnitMovement::methods[] = {
    {"__gc", UserData::Delete<::UnitMovement>},
    {"CanMove", UnitMovement::CanMove},
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