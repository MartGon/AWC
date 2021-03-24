#include <Script/UserData/MovementDescType.h>
#include <Script/UserData/UserData.h>
#include <Script/Wrappers/LuaTable.h>

#include <Script/UserData/AreaDesc.h>
#include <Script/UserData/CostTable.h>

using namespace Script;

const char* UserData::MovementDescType::MT_NAME = "AWC_MovementDescType";
const char* UserData::MovementDescType::LIB_NAME = "MovementDescType";

const luaL_Reg UserData::MovementDescType::methods[] = {
    {NULL, NULL}
};
const luaL_Reg UserData::MovementDescType::functions[] = {
    {"New", MovementDescType::New},
    {NULL, NULL}
};

int UserData::MovementDescType::New(lua_State* luaState)
{
    bool isTable = lua_istable(luaState, 1);
    luaL_argcheck(luaState, isTable, 1, "Expected table");

    FromTable(luaState, 1);

    return 1;
}

UserData::MovementDescType::type* UserData::MovementDescType::FromTable(lua_State* luaState, int index)
{
    LuaTable<Scope::Internal> lt{luaState, index};

    auto tpd = lt.GetUserData<AreaDesc>("tpd");

    auto range = lt.GetLuaWrapper<Script::LuaTable<Scope::Internal>>("range");
    luaL_argcheck(luaState, range, 1, "range table was not found");

    Range r{range->Get<uint>("max"), range->Get<uint>("min")};

    auto tileCT = lt.GetLuaWrapper<Script::LuaTable<Scope::Internal>>("tileCT");
    luaL_argcheck(luaState, tileCT, 1, "tile CostTable was not found");

    ::CostTable tct = *lt.GetUserData<CostTable>("tileCT");

    auto unitCT = lt.GetLuaWrapper<Script::LuaTable<Scope::Internal>>("unitCT");
    luaL_argcheck(luaState, unitCT, 1, "unit CostTable was not found");

    ::CostTable uct = *lt.GetUserData<CostTable>("unitCT");
    
    auto maxGas = lt.Get<uint>("maxGas");

    MovementDescTypePtr mdt{ new ::MovementDescType{*tpd, r, tct, uct, maxGas}};
    return UserData::PushDataCopy<MovementDescType>(luaState, mdt);
}

/*
    MovementDescType.New({
        tpd = AreaDesc.New({ ... }),
        range = {
            "min" = 0;
            "max" = 100;
        },
        tileCT = {
            {id = 1, cost = 1}
        },
        unitCT = {
            {id = 1, cost = -1}
        },
        maxGas = -1;
    })
*/