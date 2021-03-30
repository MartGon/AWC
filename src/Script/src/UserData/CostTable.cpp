#include <Script/UserData/CostTable.h>
#include <Script/UserData/Vector2.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::CostTable::MT_NAME = "AWC_CostTable";
const char* UserData::CostTable::LIB_NAME = "CostTable";

const luaL_Reg UserData::CostTable::methods[] = {
    {NULL, NULL}
};

const luaL_Reg UserData::CostTable::functions[] = {
    {"New", CostTable::New},
    {NULL, NULL}
};

int UserData::CostTable::New(lua_State* luaState)
{
    LuaTable<Scope::Internal> entries{luaState, 1};
    unsigned int defaultCost = luaL_checkinteger(luaState, 2);

    ::CostTable ct{GetPairsTable<unsigned int>(luaState, entries, "cost"), defaultCost};

    UserData::PushDataCopy<CostTable>(luaState, ct);

    return 1;
}

UserData::CostTable::type* UserData::CostTable::FromTable(lua_State* luaState, int index)
{
    LuaTable<Scope::Internal> t{luaState, index};

    auto defaultCost = t.ContainsValue("default") ? t.Get<unsigned int>("default") : 1;
    auto entries = t.GetLuaWrapper<LuaTable<Scope::Internal>>("entries");

    ::CostTable ct{GetPairsTable<unsigned int>(luaState, entries.value(), "cost"), defaultCost};

    return UserData::PushDataCopy<CostTable>(luaState, ct);
}