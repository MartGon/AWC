#include <Script/UserData/Unit.h>
#include <Script/UserData/UserData.h>

#include <Script/UserData/Map.h>
#include <Script/UserData/Vector2.h>
#include <Script/UserData/UnitMovement.h>

using namespace Script;

const char* UserData::Unit::MT_NAME = "AWC_Unit";

const luaL_Reg UserData::Unit::methods[] = {
    {"CaculateMovement", Unit::CalculateMovement},
    {NULL, NULL}
};

void UserData::Unit::Init(lua_State* luaState)
{
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
}

void UserData::Unit::PushLight(lua_State* luaState, ::Unit* unit)
{
    UserData::PushLight(luaState, MT_NAME, unit);
}

int UserData::Unit::CalculateMovement(lua_State* luaState)
{
    auto unit = UserData::ToLightUserData<::Unit>(luaState, MT_NAME);
    auto map = UserData::ToLightUserData<::Map>(luaState, Map::MT_NAME, 2);
    auto vector2 = UserData::ToFullUserData<::Vector2>(luaState, Vector2::MT_NAME, 3);
    
    auto unitMovement = unit->CalculateMovement(*map, *vector2);
    auto unitMove = UserData::PushFullUserData(luaState, MT_NAME, unitMovement);

    return 1;
}