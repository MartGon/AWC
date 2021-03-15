#include <Script/UserData/UserData.h>

using namespace Script::UserData;

bool UserData::HasGCMethod(lua_State* luaState, const char* mtName)
{
    int type = luaL_getmetatable(luaState, mtName);
    if(type == LUA_TTABLE)
    {
        int type = lua_getfield(luaState, -1, "__gc");
        lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);

    return type == LUA_TFUNCTION;
}