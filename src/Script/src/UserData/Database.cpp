#include <Script/UserData/Database.h>

#include <Script/UserData/MovementDescType.h>

using namespace Script;

const char* UserData::Database::MT_NAME = "AWC_Database";
const char* UserData::Database::LIB_NAME = "Database";

const luaL_Reg UserData::Database::methods[] = {
    {"AddUnitType", AddUnitType},
    {NULL, NULL}
};
const luaL_Reg UserData::Database::functions[] = {
    {"Get", Get},
    {NULL, NULL}
};

int UserData::Database::Get(lua_State* luaState)
{
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); // Get _G
    LuaTable lt{luaState, -1}; // Global lua table wrapper

    auto db = lt.GetUserData<Database>("DB");
    UserData::PushDataRef<Database>(luaState, db);

    return 1;
}

int UserData::Database::AddUnitType(lua_State* luaState)
{
    auto db = UserData::ToUserData<Database>(luaState, 1);
    auto& unitTypes = db->get<::UnitType>();

    bool isTable = lua_istable(luaState, 2);
    luaL_argcheck(luaState, isTable, 2, "Expected table");

    LuaTable lt{luaState, 2};
    std::string name = lt.Get<std::string>("name");
    auto moveType = *lt.GetUserData<MovementDescType>("moveType");

    UnitType ut{unitTypes.GetIndex(), name, moveType, {}};
    unitTypes.Add(ut);

    return 0;
}
/*

    db.AddUnitType({
        name = "Rook",
        moveType = MoveDescType.New({
            ...
        }),
        weaponTypes = {
            ...
        }

    })

*/