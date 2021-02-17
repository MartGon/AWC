#include "doctest.h"

#include <lua.hpp>

#include <string>

int ExecuteLuaFile(lua_State* luaState, std::string file)
{
    auto res = luaL_loadfile(luaState, file.c_str());

    if(res == LUA_OK)
        res = lua_pcall(luaState, 0, 0, 0);

    return res;
}

TEST_CASE("Script test")
{
    bool alright = false;
    auto luaState = luaL_newstate();

    std::string testFile = std::string(SCRIPTS_DIR) + "test.lua";
    if(ExecuteLuaFile(luaState, testFile) == LUA_OK)
    {
        auto type = lua_getglobal(luaState, "TEST");
        int value = lua_tointeger(luaState, -1);
        alright = value == 3;
    }

    lua_close(luaState);

    CHECK(alright == true);
}

class TestMap
{
public:
    TestMap(int x, int y) : width{x}, height{y}, cells{new int[x*y]}
    {
    }

    ~TestMap()
    {
        delete[] cells;
    }

    int GetValue(int x, int y)
    {
        auto val = 0;

        if(IsIndexValid(x, y))
            val = cells[GetIndex(x, y)];

        return val;
    }

    int SetValue(int x, int y, int value)
    {
        if(IsIndexValid(x, y))
            cells[GetIndex(x, y)] = value;

        return 0;
    }

private:

    int GetIndex(int x, int y)
    {
        return x * height + y;
    }

    bool IsIndexValid(int x, int y)
    {
        return x >= 0 && x < width && y >=0 && y < height;
    }

    int* cells;
    int height;
    int width;
};

int GetMap(lua_State* L)
{   
    auto map = lua_touserdata(L, lua_upvalueindex(1));
    lua_pushlightuserdata(L, map);

    luaL_getmetatable(L, "AWC_Map");
    lua_setmetatable(L, -2);

    return 1;
}

int SetMapValue(lua_State* L)
{
    auto map = static_cast<TestMap*>(lua_touserdata(L, 1));
    auto x = lua_tointeger(L, 2);
    auto y = lua_tointeger(L, 3);
    auto value = lua_tointeger(L, 4);

    map->SetValue(x, y, value);

    return 0;
}

int GetMapValue(lua_State* L)
{
    auto pointer = lua_touserdata(L, 1);
    auto map = static_cast<TestMap*>(pointer);
    auto x = luaL_checkinteger(L, 2);
    auto y = luaL_checkinteger(L, 3);

    auto value = map->GetValue(x, y);
    lua_pushinteger(L, value);

    return 1;
}

TEST_CASE("User data Test")
{
    TestMap map{5, 5};
    map.SetValue(0, 1, 10);

    auto luaState = luaL_newstate();

    lua_pushlightuserdata(luaState, &map);
    lua_pushcclosure(luaState, GetMap, 1);
    lua_setglobal(luaState, "GetMap");

    const luaL_Reg methods[] = {
        {"SetValue", SetMapValue},
        {"GetValue", GetMapValue},
        {NULL, NULL}
    };

    // Create Map object metatable
    luaL_newmetatable(luaState, "AWC_Map");

    lua_pushstring(luaState, "__index");
    lua_pushvalue(luaState, -2);
    lua_settable(luaState, -3);

    luaL_setfuncs(luaState, methods, 0);

    void* lMap = 0;
    int value = -1;
    std::string file = std::string(SCRIPTS_DIR) + "userdata.lua";
    if(ExecuteLuaFile(luaState, file) == LUA_OK)
    {
        auto type = lua_getglobal(luaState, "map");
        lMap = lua_touserdata(luaState, -1);

        type = lua_getglobal(luaState, "value");
        value = lua_tointeger(luaState, -1);
    }

    lua_close(luaState);

    CHECK(lMap == (void*)&map);
    CHECK(value == map.GetValue(0, 1));
}

struct TestOperation
{
    TestOperation(uint id, std::string name, int executeRef) : id{id}, name{name}, executeRef{executeRef}
    {

    }

    void Execute(lua_State* luaState)
    {
        auto type = lua_rawgeti(luaState, LUA_REGISTRYINDEX, executeRef);
        if(type == LUA_TFUNCTION)
        {
            auto res = lua_pcall(luaState, 0, 0, 0);
        }
    }

    uint id;
    std::string name;

    int executeRef;
    int dataRef;
};

TEST_CASE("Custom operation test")
{
    auto luaState = luaL_newstate();

    lua_pushinteger(luaState, 15);
    lua_setglobal(luaState, "value");

    std::string file = std::string(SCRIPTS_DIR) + "optest.lua";

    int regIndex = 0;
    if(LUA_OK == luaL_loadfile(luaState, file.c_str()))
    {  
        regIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
    }
    TestOperation top{0, "testOp", regIndex};

    top.Execute(luaState);
    lua_getglobal(luaState, "value");
    int value = lua_tointeger(luaState, -1);

    CHECK(value == 20);

    top.Execute(luaState);
    lua_getglobal(luaState, "value");
    value = lua_tointeger(luaState, -1);

    CHECK(value == 25);

    lua_close(luaState);
}