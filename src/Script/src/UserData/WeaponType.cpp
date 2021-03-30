#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::WeaponType::MT_NAME = "AWC_WeaponType";
const char* UserData::WeaponType::LIB_NAME = "WeaponType";

const luaL_Reg UserData::WeaponType::methods[] = {
    {NULL, NULL}
};
const luaL_Reg UserData::WeaponType::functions[] = {
    {"New", WeaponType::New},
    {NULL, NULL}
};

int UserData::WeaponType::New(lua_State* luaState)
{
    bool isTable = lua_istable(luaState, 1);
    luaL_argcheck(luaState, isTable, 1, "Expected table");

    FromTable(luaState, 1);

    return 1;
}

void FillTable(lua_State* luaState, ::DamageTable& at, LuaTable<Scope::Internal>& entries)
{
    for(int i = 0; i < entries.Length(); i++)
    {
        int luaI = i + 1;
        auto entry = entries.GetLuaWrapper<Script::LuaTable<Scope::Internal>>(i + 1);
        luaL_argcheck(luaState, entry, 1, "table entry in AttackTable was not a table");

        at.SetDamageToUnit(entry->Get<uint>("id"), entry->Get<float>("cost"));
    }
}

UserData::WeaponType::type* UserData::WeaponType::FromTable(lua_State* luaState, int index)
{
    LuaTable<Scope::Internal> lt{luaState, index};

    auto tpd = *lt.GetUserData<AreaDesc>("tpd");

    auto range = lt.GetLuaWrapper<Script::LuaTable<Scope::Internal>>("range");
    luaL_argcheck(luaState, range, 1, "range table was not found");

    Range r{range->Get<uint>("max"), range->Get<uint>("min")};

    auto lat = lt.GetLuaWrapper<Script::LuaTable<Scope::Internal>>("attackTable").value();
    AttackTable attackTable{GetPairsTable<bool, Scope::Internal>(luaState, lat, "attackable")};

    auto ldt = lt.GetLuaWrapper<Script::LuaTable<>>("dmgTable").value();
    DamageTable dmgTable{GetPairsTable<float, Scope::Internal>(luaState, ldt, "dmg")};

    ::WeaponTypePtr weaponType = std::make_shared<::WeaponType>(tpd, r, attackTable, dmgTable, -1);

    return UserData::PushDataCopy<WeaponType>(luaState, weaponType);;
}

/*
    MovementDescType.New({
        tpd = AreaDesc.New({ ... }),
        range = {
            min = 0;
            max = 100;
        },
        attackTable = {
            {id = 1, attackable = 1}
        },
        dmgTable = {
            {id = 1, dmg = 1}
        },
        maxAmmo = -1;
    })
*/