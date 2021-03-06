#include "doctest.h"

#include <lua.hpp>

#include <string>

#include <AWC.h>
#include <Script.h>

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
    lua_settable(luaState, -3); // Metatable.__index = Metatable

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

TEST_CASE("Custom environment test")
{
    auto luaState = luaL_newstate();

    bool fileLoadedFine = false;
    bool valueIsNil = false;
    bool valueIsStillNil = false;
    bool envValueIsFine = false;
    bool envNewValueIsFine = false;
    bool changedEnvValueOk = false;

    std::string file = std::string(SCRIPTS_DIR) + "environment.lua";
    if(LUA_OK == luaL_loadfile(luaState, file.c_str()))
    {
        // Creates table and gets a ref
        lua_newtable(luaState);
        auto envRef = luaL_ref(luaState, LUA_REGISTRYINDEX);

        // Sets that table as ENV
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, envRef);
        auto ret = lua_setupvalue(luaState, 1, 1);
        if(LUA_OK == lua_pcall(luaState, 0, 0, 0))
        {
            // Get table again
            lua_rawgeti(luaState, LUA_REGISTRYINDEX, envRef);

            // Get Execute func
            lua_pushstring(luaState, "Execute");
            lua_rawget(luaState, -2);

            // Check if type at top is a function
            auto type = lua_type(luaState, -1);
            fileLoadedFine = type == LUA_TFUNCTION;

            // This global should be nil
            lua_getglobal(luaState, "value");
            valueIsNil = lua_isnil(luaState, -1);
            lua_pop(luaState, 1);

            // This env var should be 5
            lua_getfield(luaState, -2, "value");
            auto value = lua_tointeger(luaState, -1);
            envValueIsFine = value == 5;
            lua_pop(luaState, 1);

            // Call the function, it should be at top
            if(LUA_OK == lua_pcall(luaState, 0, 0, 0))
            {
                // Now the ENV table is at top
                lua_getfield(luaState, -1, "value");
                value = lua_tointeger(luaState, -1);
                envNewValueIsFine = value == 3;
                lua_pop(luaState, 1);

                // This global should still be nil
                lua_getglobal(luaState, "value");
                valueIsStillNil = lua_isnil(luaState, -1);
                lua_pop(luaState, 1);

                // ----- Changing ENV -------

                // Get table where Execute func is defined
                lua_rawgeti(luaState, LUA_REGISTRYINDEX, envRef);

                // Get Execute func
                lua_pushstring(luaState, "Execute");
                lua_rawget(luaState, -2);

                // Create new ENV table
                lua_newtable(luaState);
                
                // Set value to 10
                lua_pushinteger(luaState, 10);
                lua_setfield(luaState, -2, "value");
                
                // Change ENV table of the Execute function.
                // As of Lua 5.4, functions always have ENV as their first upvalue
                lua_setupvalue(luaState, -2, 1);
                
                // Call the execute function, it should be at top
                if(LUA_OK == lua_pcall(luaState, 0, 0, 0))
                {
                    // Get table where Execute func is defined
                    lua_rawgeti(luaState, LUA_REGISTRYINDEX, envRef);

                    // Get Execute func
                    lua_pushstring(luaState, "Execute");
                    lua_rawget(luaState, -2);

                    // Get ENV table
                    lua_getupvalue(luaState, -1, 1);

                    // Check value variable, ENV table should be now at top
                    lua_getfield(luaState, -1, "value");
                    value = lua_tointeger(luaState, -1);
                    changedEnvValueOk = value == 8;
                    lua_pop(luaState, 1);
                }
            }
        }
    }

    CHECK(fileLoadedFine == true);
    CHECK(valueIsNil == true);
    CHECK(valueIsStillNil == true);
    CHECK(envValueIsFine == true);
    CHECK(envNewValueIsFine == true);
    CHECK(changedEnvValueOk == true);

    lua_close(luaState);
}

#include <iostream>
#include <stdlib.h>

TEST_CASE("GC")
{
    struct Test
    {
        Test(std::shared_ptr<int> a) : a{a}
        {
            std::cout << "Calling constructor of Test\n" ;
        }

        ~Test()
        {
            std::cout << "Calling destructor of Test\n";
            std::cout << "Count is " << a.use_count() << "\n";
        }

        std::shared_ptr<int> a;
    };

    auto aPtr = std::make_shared<int>(2); // Count = 1;

    Test* t = (Test*)malloc(sizeof(Test)); // Count = 2
    *t = Test{aPtr};

    CHECK(aPtr.use_count() == 2);

    t->~Test(); // This is needed to make Count = 1;

    CHECK(aPtr.use_count() == 1);

    free(t); // Free memory

    // Implicit Count = 0;
}

#include <Test/Script/Script.h>

TEST_CASE("ENV with __index _G")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    std::string path1 = std::string(SCRIPTS_DIR) + "Global1.lua";
    Test::Script::TestScript ts1(path1, sGame);

    std::string path2 = std::string(SCRIPTS_DIR) + "Global2.lua";
    Test::Script::TestScript ts2(path2, sGame);
    
    sGame.PushScript(ts1.ref);
    sGame.PushScript(ts2.ref);
    game.Run();

    // Ts1 Table
    auto& ts1Table = sGame.GetScriptTable(ts1.ref);
    auto global1 = ts1Table.GetInt("global");
    auto var1 = ts1Table.GetInt("var1");
    auto var2 = ts1Table.GetInt("var2");

    CHECK(global1 == 15);
    CHECK(var1 == 12);
    CHECK(var2 == 0); // Nil, not defined

    // Ts2 Table
    auto& ts2Table = sGame.GetScriptTable(ts2.ref);
    auto global2 = ts2Table.GetInt("global");
    var1 = ts2Table.GetInt("var1");
    var2 = ts2Table.GetInt("var2");

    CHECK(global2 == 4);
    CHECK(var1 == 0); // Nil, not defined
    CHECK(var2 == 3);
}