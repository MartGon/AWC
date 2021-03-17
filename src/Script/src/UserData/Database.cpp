#include <Script/UserData/Database.h>

using namespace Script;

const char* UserData::Database::MT_NAME = "AWC_Database";
const char* UserData::Database::LIB_NAME = "Database";

const luaL_Reg UserData::Database::methods[] = {
    {NULL, NULL}
};
const luaL_Reg UserData::Database::functions[] = {
    {"Get", Get},
    {NULL, NULL}
};

int UserData::Database::Get(lua_State* luaState)
{
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); // Get _G
    LuaTable lt{luaState, -1};
    auto db = lt.GetUserData<Database>("DB");
    UserData::PushDataRef<Database>(luaState, db);

    return 1;
}