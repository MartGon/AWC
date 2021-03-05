#include <Script/UserData/UserData.h>

using namespace Script::UserData;

void UserData::RegisterMetatable(lua_State* luaState, const char* mtName, const luaL_Reg* methods)
{
    luaL_newmetatable(luaState, mtName);

    lua_pushstring(luaState, "__index");
    lua_pushvalue(luaState, -2);
    lua_settable(luaState, -3); // Metatable.__index = Metatable

    luaL_setfuncs(luaState, methods, 0);
    lua_pop(luaState, 1);
}

void UserData::RegisterMetaMethod(lua_State* luaState, const char* mtName, const char* key, const lua_CFunction method)
{
    luaL_getmetatable(luaState, mtName);
    lua_pushcfunction(luaState, method);
    lua_setfield(luaState, -2, key);
    lua_pop(luaState, 1);
}

void UserData::RegisterLib(lua_State* luaState, const char* libName, const luaL_Reg* funcs)
{
    lua_newtable(luaState);
    luaL_setfuncs(luaState, funcs, 0);
    lua_setglobal(luaState, libName);
}

bool UserData::HasGCMethod(lua_State* luaState, const char* mtName)
{
    luaL_getmetatable(luaState, mtName);
    int type = lua_getfield(luaState, -1, "__gc");
    lua_pop(luaState, 2);

    return type != LUA_TNIL;
}