#include <Script/UserData/Vector2.h>
#include <Script/UserData/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::Vector2::MT_NAME = "Utils_Vector2";
const char* UserData::Vector2::LIB_NAME = "Vector2";

const luaL_Reg UserData::Vector2::methods[] = {
    {"__add", Vector2::Add},
    {"__sub", Vector2::Sub},
    {"__eq", Vector2::Eq},
    {"__tostring", Vector2::ToString},
    {"__index", Vector2::Get},
    {"__newindex", Vector2::Set},
    {NULL, NULL}
};

const luaL_Reg UserData::Vector2::functions[] = {
    {"new", Vector2::New},
    {NULL, NULL}
};

int UserData::Vector2::New(lua_State* luaState)
{
    int x = luaL_checkinteger(luaState, 1);
    int y = luaL_checkinteger(luaState, 2);
    ::Vector2* vec = UserData::PushDataCopy<Vector2>(luaState, ::Vector2{x, y});

    return 1;
}

::Vector2* UserData::Vector2::FromTable(lua_State* luaState, int index)
{
    LuaTable table{luaState, index};
    auto x = table.Get<int>("x");
    auto y = table.Get<int>("y");
    ::Vector2* vec = UserData::PushDataCopy<Vector2>(luaState, ::Vector2{x, y});

    return vec;
}

int UserData::Vector2::Get(lua_State* luaState)
{
    ::Vector2* vec = UserData::ToUserData<Vector2>(luaState, 1);
    std::string index = std::string(luaL_checkstring(luaState, 2));

    if(index == "x")
        lua_pushinteger(luaState, vec->x);
    else if( index == "y")
        lua_pushinteger(luaState, vec->y);
    else
        luaL_error(luaState, "Invalid index %s", index.c_str());

    return 1;
}

int UserData::Vector2::Set(lua_State* luaState)
{
    ::Vector2* vec = UserData::ToUserData<Vector2>(luaState, 1);
    std::string index = std::string(luaL_checkstring(luaState, 2));
    auto n = luaL_checkinteger(luaState, 3);

    if(index == "x")
        vec->x = n;
    else if( index == "y")
        vec->y = n;
    else
        luaL_error(luaState, "Invalid index %s", index.c_str());

    return 1;
}

int UserData::Vector2::Add(lua_State* luaState)
{
    ::Vector2* a = UserData::ToUserData<Vector2>(luaState, 1);
    ::Vector2* b = UserData::ToUserData<Vector2>(luaState, 2);

    ::Vector2 sum = *a + *b;
    UserData::PushDataCopy<Vector2>(luaState, sum);

    return 1;
}

int UserData::Vector2::Sub(lua_State* luaState)
{
    ::Vector2* a = UserData::ToUserData<Vector2>(luaState, 1);
    ::Vector2* b = UserData::ToUserData<Vector2>(luaState, 2);

    ::Vector2 sub = *a - *b;
    UserData::PushDataCopy<Vector2>(luaState, sub);

    return 1;
}

int UserData::Vector2::Eq(lua_State* luaState)
{
    ::Vector2* a = UserData::ToUserData<Vector2>(luaState, 1);
    ::Vector2* b = UserData::ToUserData<Vector2>(luaState, 2);

    bool eq = *a == *b;
    lua_pushboolean(luaState, eq);

    return 1;
}

int UserData::Vector2::ToString(lua_State* luaState)
{
    ::Vector2* a = UserData::ToUserData<Vector2>(luaState, 1);

    std::string str = a->ToString();
    lua_pushstring(luaState, str.c_str());

    return 1;
}