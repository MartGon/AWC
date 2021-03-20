#include <Script/UserData/MovementDescType.h>
#include <Script/UserData/UserData.h>
#include <Script/Wrappers/LuaTable.h>

#include <Script/UserData/AreaDesc.h>

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

CostTable ParseCostTable(lua_State* luaState, LuaTable& t)
{
    CostTable ct{{}, std::numeric_limits<unsigned int>::max()};
    for(int i = 0; i < t.Length(); i++)
    {
        int luaI = i + 1;
        auto entry = t.GetLuaWrapper<Script::LuaTable>(i + 1);
        luaL_argcheck(luaState, entry, 1, "table entry in CostTable was not a table");

        ct.SetCost(entry->Get<uint>("id"), entry->Get<uint>("cost"));
    }

    return ct;
}

int UserData::MovementDescType::New(lua_State* luaState)
{
    bool isTable = lua_istable(luaState, 1);
    luaL_argcheck(luaState, isTable, 1, "Expected table");

    LuaTable lt{luaState, 1};

    auto tpd = lt.GetUserData<AreaDesc>("tpd");

    auto range = lt.GetLuaWrapper<Script::LuaTable>("range");
    luaL_argcheck(luaState, range, 1, "range table was not found");

    Range r{range->Get<uint>("max"), range->Get<uint>("min")};

    auto tileCT = lt.GetLuaWrapper<Script::LuaTable>("tileCT");
    luaL_argcheck(luaState, tileCT, 1, "tile CostTable was not found");

    CostTable tct = ParseCostTable(luaState, *tileCT);

    auto unitCT = lt.GetLuaWrapper<Script::LuaTable>("unitCT");
    luaL_argcheck(luaState, unitCT, 1, "unit CostTable was not found");
    CostTable uct = ParseCostTable(luaState, *unitCT);

    auto maxGas = lt.Get<uint>("maxGas");

    MovementDescTypePtr mdt{ new ::MovementDescType{*tpd, r, tct, uct, maxGas}};
    UserData::PushDataCopy<MovementDescType>(luaState, mdt);

    return 1;
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