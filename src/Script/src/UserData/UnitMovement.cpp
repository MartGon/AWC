#include <Script/UserData/UnitMovement.h>
#include <Script/UserData/UserData.h>

using namespace Script;

const char* UserData::UnitMovement::MT_NAME = "AWC_UnitMovement";
const luaL_Reg UserData::UnitMovement::methods[] = {
    {NULL, NULL}
};

void UserData::UnitMovement::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}