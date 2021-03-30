#include <Script/UserData/Database.h>

#include <Script/UserData/MovementDescType.h>
#include <Script/UserData/UnitType.h>

using namespace Script;

const char* UserData::Database::MT_NAME = "AWC_Database";
const char* UserData::Database::LIB_NAME = "Database";

const luaL_Reg UserData::Database::methods[] = {
    {"AddUnitType", AddUnitType},
    {"GetUnitType", GetUnitType},
    {"AddTileType", AddTileType},
    {"GetTileType", GetTileType},
    {NULL, NULL}
};
const luaL_Reg UserData::Database::functions[] = {
    {"Get", Get},
    {NULL, NULL}
};

int UserData::Database::Get(lua_State* luaState)
{
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); // Get _G
    LuaTable<Scope::Internal> lt{luaState, -1}; // Global lua table wrapper

    auto db = lt.GetUserData<Database>("DB");
    UserData::PushDataRef<Database>(luaState, db);

    return 1;
}

int UserData::Database::AddUnitType(lua_State* luaState)
{
    auto db = UserData::CheckUserData<Database>(luaState, 1);
    auto& unitTypes = db->get<::UnitType>();

    bool isTable = lua_istable(luaState, 2);
    luaL_argcheck(luaState, isTable, 2, "Expected table");

    LuaTable<Scope::Internal> lt{luaState, 2};
    std::string name = lt.Get<std::string>("name");
    auto moveType = *lt.GetUserData<MovementDescType>("moveType");

    std::vector<WeaponType::type> weapons;
    if(lt.ContainsValue("weapons"))
    {
        auto weaponTypesTable = lt.GetLuaWrapper<LuaTable<>>("weapons");
        auto len = weaponTypesTable->Length();
        for(int i = 1; i < len + 1; i++)
            weapons.push_back(*weaponTypesTable->GetUserData<WeaponType>(i));
    }

    ::UnitType ut{unitTypes.GetFreeId(), name, moveType, weapons};
    auto id = unitTypes.Add(ut);
    auto ptr = unitTypes.GetById(id);
    UserData::PushDataRef<UnitType>(luaState, ptr);

    return 1;
}

int UserData::Database::GetUnitType(lua_State* luaState)
{
    auto db = UserData::CheckUserData<Database>(luaState, 1);
    auto& unitTypes = db->get<::UnitType>();

    unsigned int typeId = luaL_checkinteger(luaState, 2);
    auto unitType = unitTypes.GetById(typeId);

    if(unitType)
        UserData::PushDataRef<UnitType>(luaState, unitType);
    else
        lua_pushnil(luaState);

    return 1;
}

int UserData::Database::AddTileType(lua_State* luaState)
{
    auto db = UserData::CheckUserData<Database>(luaState, 1);
    auto& tileTypes = db->get<::TileType>();
    auto id = tileTypes.GetFreeId();

    auto name = std::string(luaL_checkstring(luaState, 2));
    tileTypes.Add(::TileType{id, name});

    UserData::PushDataRef<TileType>(luaState, tileTypes.GetById(id));

    return 1;
}

int UserData::Database::GetTileType(lua_State* luaState)
{
    auto db = UserData::CheckUserData<Database>(luaState, 1);
    auto& tileTypes = db->get<::TileType>();

    auto id = luaL_checkinteger(luaState, 2);
    auto tileType = tileTypes.GetById(id);

    if(tileType)
        UserData::PushDataRef<TileType>(luaState, tileTypes.GetById(id));
    else
        lua_pushnil(luaState);

    return 1;
}