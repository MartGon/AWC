#include <Script/UserData/UnitMovement.h>

#include <Script/UserData/UserData.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::UnitMovement::MT_NAME = "AWC_UnitMovement";
const luaL_Reg UserData::UnitMovement::methods[] = {
    {"__gc", UserData::Delete<::UnitMovement*>},
    {"CanMove", UnitMovement::CanMove},
    {NULL, NULL}
};

void UserData::UnitMovement::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}

int UserData::UnitMovement::CanMove(lua_State* luaState)
{
    auto unitMove = UserData::ToFullUserData<::UnitMovement>(luaState, MT_NAME);
    auto dest = UserData::ToFullUserData<::Vector2>(luaState, Vector2::MT_NAME, 2);

    bool result = unitMove->CanMove(*dest);
    lua_pushboolean(luaState, result);

    return 1;
}