#include <Script/UserData/Unit.h>
#include <Script/UserData/UserData.h>
#include <Script/UserData/Map.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::Unit::MT_NAME = "AWC_Unit";

const luaL_Reg UserData::Unit::methods[] = {
    {NULL, NULL}
};

void UserData::Unit::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}

void UserData::Unit::Push(lua_State* luaState, ::Unit* unit)
{
    UserData::Push(luaState, MT_NAME, unit);
}

int UserData::Unit::CalculateMovement(lua_State* luaState)
{
    auto unit = UserData::ToUserData<::Unit>(luaState, -1, MT_NAME);
    auto map = Map::ToMap(luaState, -2);
    auto vector2 = Vector2::ToVector2(luaState, -1);
    
    auto unitMovement = unit->CalculateMovement(*map, *vector2);
}