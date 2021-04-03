#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::UnitType::MT_NAME = "AWC_UniType";
const char* UserData::UnitType::LIB_NAME = "UnitType";
const luaL_Reg UserData::UnitType::methods[] = {
    {"CreateUnit", UnitType::CreateUnit},
    {"AddEventHandler", UnitType::AddEventHandler},
    {"GetId", UnitType::GetId},
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
    
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); // Get _G
    LuaTable<Scope::Internal> gTable{luaState, -1}; // Global lua table wrapper

    // Register handlers
    auto game = gTable.GetUserData<Game>("game");
    auto& subject = game->GetSubject();
    unit->RegisterHandlers(subject);

    UserData::PushDataCopy<Unit>(luaState, unit);

    return 1;
}

int UserData::UnitType::AddEventHandler(lua_State* luaState)
{
    auto unitType = UserData::CheckUserData<UnitType>(luaState, 1);
    auto eh = *UserData::CheckUserData<EventHandler>(luaState, 2);

    unitType->AddHandler(eh);

    return 0;
}

int UserData::UnitType::GetId(lua_State* luaState)
{
    auto unitType = UserData::CheckUserData<UnitType>(luaState, 1);

    lua_pushinteger(luaState, unitType->GetId());

    return 1;
}
