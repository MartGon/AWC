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

void UserData::Push(lua_State* luaState, const char* mtName, void* userdata)
{
    lua_pushlightuserdata(luaState, userdata);
    luaL_getmetatable(luaState, mtName);
    lua_setmetatable(luaState, -2);
}