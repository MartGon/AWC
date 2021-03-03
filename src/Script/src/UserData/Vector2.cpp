#include <Script/UserData/Vector2.h>
#include <Script/UserData/UserData.h>

using namespace Script;

const char* UserData::Vector2::MT_NAME = "Utils_Vector2";
const char* UserData::Vector2::LIB_NAME = "Vector2";

const luaL_Reg UserData::Vector2::methods[] = {
    {NULL, NULL}
};

const luaL_Reg UserData::Vector2::functions[] = {
    {"new", Vector2::New},
    {NULL, NULL}
};

void UserData::Vector2::Init(lua_State* luaState)
{
    luaL_newmetatable(luaState, MT_NAME);

    lua_pushstring(luaState, "__index");
    lua_pushcfunction(luaState, Vector2::Get);
    lua_settable(luaState, -3); // Metatable.__index = Get

    lua_pushstring(luaState, "__gc");
    lua_pushcfunction(luaState, UserData::Delete<::Vector2>);
    lua_settable(luaState, -3);

    lua_pop(luaState, 1);

    UserData::RegisterLib(luaState, LIB_NAME, functions);
}

int UserData::Vector2::New(lua_State* luaState)
{
    int x = luaL_checkinteger(luaState, 1);
    int y = luaL_checkinteger(luaState, 2);
    ::Vector2* vec = UserData::PushFullUserData<::Vector2>(luaState, MT_NAME, ::Vector2{x, y});

    luaL_setmetatable(luaState, MT_NAME);

    return 1;
}

int UserData::Vector2::Get(lua_State* luaState)
{
    ::Vector2* vec = UserData::ToFullUserData<::Vector2>(luaState, MT_NAME);
    std::string index = std::string(luaL_checkstring(luaState, 2));

    bool isIndexValid = index == "x" || index == "y";
    if(isIndexValid)
    {
        if(index == "x")
            lua_pushinteger(luaState, vec->x);
        else
            lua_pushinteger(luaState, vec->y);
    }
    else
        luaL_error(luaState, "Invalid index %s", index.c_str());

    return 1;
}