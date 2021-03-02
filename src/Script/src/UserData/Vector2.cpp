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
    UserData::RegisterMetatable(luaState, MT_NAME, methods);
    UserData::RegisterLib(luaState, LIB_NAME, functions);
}

void UserData::Vector2::Push(lua_State* luaState, ::Vector2* vec)
{
    UserData::Push(luaState, MT_NAME, vec);
}

int UserData::Vector2::New(lua_State* luaState)
{
    int x = luaL_checkinteger(luaState, -1);
    int y = luaL_checkinteger(luaState, -2);
    ::Vector2* vec = UserData::ToFullUserData<::Vector2>(luaState);
    vec->x = x;
    vec->y = y;

    luaL_getmetatable(luaState, MT_NAME);
    lua_setmetatable(luaState, -2);

    return 1;
}

::Vector2* UserData::Vector2::ToVector2(lua_State* luaState, int index)
{
    return UserData::ToUserData<::Vector2>(luaState, index, MT_NAME);
}