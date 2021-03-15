#include <Script/UserData/Unit.h>
#include <Script/UserData/UserData.h>

#include <Script/UserData/Map.h>
#include <Script/UserData/Vector2.h>
#include <Script/UserData/UnitMovement.h>

using namespace Script;

const char* UserData::Unit::MT_NAME = "AWC_Unit";

const luaL_Reg UserData::Unit::methods[] = {
    {"CalculateMovement", Unit::CalculateMovement},
    {NULL, NULL}
};

int UserData::Unit::CalculateMovement(lua_State* luaState)
{
    auto unit = *UserData::ToUserData<Unit>(luaState, 1);
    auto map = UserData::ToUserData<Map>(luaState, 2);
    auto vector2 = UserData::ToUserData<Vector2>(luaState, 3);

    auto unitMovement = unit->CalculateMovement(*map, *vector2);
    auto unitMove = UserData::PushDataCopy<UnitMovement>(luaState, unitMovement);

    return 1;
}