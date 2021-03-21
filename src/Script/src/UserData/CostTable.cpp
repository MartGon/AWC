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

void FillTable(lua_State* luaState, ::CostTable& ct, LuaTable& entries)
{
    
    for(int i = 0; i < entries.Length(); i++)
    {
        int luaI = i + 1;
        auto entry = entries.GetLuaWrapper<Script::LuaTable>(i + 1);
        luaL_argcheck(luaState, entry, 1, "table entry in CostTable was not a table");

        ct.SetCost(entry->Get<uint>("id"), entry->Get<uint>("cost"));
    }
}

int UserData::CostTable::New(lua_State* luaState)
{
    LuaTable entries{luaState, 1};
    unsigned int defaultCost = luaL_checkinteger(luaState, 2);

    ::CostTable ct{{}, defaultCost};
    FillTable(luaState, ct, entries);

    UserData::PushDataCopy<CostTable>(luaState, ct);

    return 1;
}

UserData::CostTable::type* UserData::CostTable::FromTable(lua_State* luaState, int index)
{
    LuaTable t{luaState, index};

    auto defaultCost = t.ContainsValue("default") ? t.Get<unsigned int>("default") : 1;
    auto entries = t.GetLuaWrapper<LuaTable>("entries");

    ::CostTable ct{{}, defaultCost};
    FillTable(luaState, ct, *entries);

    return UserData::PushDataCopy<CostTable>(luaState, ct);
}