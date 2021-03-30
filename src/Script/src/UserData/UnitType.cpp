#include <Script/UserData/UnitType.h>
#include <Script/UserData/UserData.h>

#include <Script/UserData/Player.h>
#include <Script/UserData/Unit.h>

using namespace Script;

const char* UserData::UnitType::MT_NAME = "AWC_UniType";
const char* UserData::UnitType::LIB_NAME = "UnitType";
const luaL_Reg UserData::UnitType::methods[] = {
    {"CreateUnit", UnitType::CreateUnit},
    {NULL, NULL}
};
const luaL_Reg UserData::UnitType::functions[] = {
    {NULL, NULL}
};

int UserData::UnitType::CreateUnit(lua_State* luaState)
{
    auto unitType = UserData::CheckUserData<UnitType>(luaState, 1);
    auto player = UserData::CheckUserData<Player>(luaState, 2);

    auto unit = unitType->CreateUnit(player);
    UserData::PushDataCopy<Unit>(luaState, unit);

    return 1;
}
