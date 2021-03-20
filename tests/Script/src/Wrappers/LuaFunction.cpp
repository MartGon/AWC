#include "doctest.h"

#include <AWC.h>
#include <Script.h>

TEST_CASE("Construction")
{
    auto L = luaL_newstate();
    auto baseLen = luaL_len(L, LUA_REGISTRYINDEX);

    SUBCASE("By index at stack")
    {
        // No function at that index
        CHECK_THROWS_AS(Script::LuaFunction(L, -1), const AWCException&);

        luaL_loadstring(L, "i = 2 + 3");
        CHECK_NOTHROW(Script::LuaFunction{L, -1});

        // Function still at top
        CHECK(lua_gettop(L) == 1);
    }
    SUBCASE("By index at table")
    {
        lua_pushnil(L);
        // No table found at given index
        CHECK_THROWS_AS(Script::LuaFunction(L, -1, "f"), const AWCException&);
        lua_pop(L, 1);

        Script::LuaTable t{L};
        t.PushLuaTable();
        // No function found at t["f"]
        CHECK_THROWS_AS(Script::LuaFunction(L, -1, "f"), const AWCException&);
        lua_pop(L, 1); 
        CHECK(lua_gettop(L) == 0);

        luaL_dostring(L, "return {f = function() i = 2 + 3; end};");
        CHECK(lua_gettop(L) == 1);
        CHECK_NOTHROW(Script::LuaFunction(L, -1, "f"));
    }
    SUBCASE("By Copy")
    {
        luaL_loadstring(L, "i = 1; i = i + 1");
        CHECK(lua_gettop(L) == 1);
        auto rLen = luaL_len(L, LUA_REGISTRYINDEX);

        Script::LuaFunction f{L, -1};
        lua_pop(L, 1);
        CHECK(lua_gettop(L) == 0);

        // Check new entry on Registry
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) > rLen);
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        // Create a copy
        Script::LuaFunction f2 = f;
        // Check new entry on registry
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) > rLen);
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        // Call both functions
        f.PushFunction();
        lua_pcall(L, 0, 0, 0);

        auto type = lua_getglobal(L, "i");
        CHECK(type == LUA_TNUMBER);
        CHECK(lua_tointeger(L, -1) == 2);

        f2.PushFunction();
        lua_pcall(L, 0, 0, 0);

        type = lua_getglobal(L, "i");
        CHECK(type == LUA_TNUMBER);
        CHECK(lua_tointeger(L, -1) == 2);
    }
    SUBCASE("Copy assignment")
    {
        luaL_loadstring(L, "i = 1; i = i + 1");
        CHECK(lua_gettop(L) == 1);
        auto rLen = luaL_len(L, LUA_REGISTRYINDEX);

        Script::LuaFunction f{L, -1};
        lua_pop(L, 1);
        CHECK(lua_gettop(L) == 0);

        // Check new entry on Registry
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) > rLen);
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        luaL_loadstring(L, "i = 1; i = i + 4");
        Script::LuaFunction g{L, -1};
        lua_pop(L, 1);
        CHECK(lua_gettop(L) == 0);

        // Check new entry on Registry
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) > rLen);
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        g = f;
        // Now g points to the function that was held in f
        // The function held by g was destroyed
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) < rLen);

        // Call both functions, both should have the same results
        f.PushFunction();
        lua_pcall(L, 0, 0, 0);

        auto type = lua_getglobal(L, "i");
        CHECK(type == LUA_TNUMBER);
        CHECK(lua_tointeger(L, -1) == 2);

        g.PushFunction();
        lua_pcall(L, 0, 0, 0);

        type = lua_getglobal(L, "i");
        CHECK(type == LUA_TNUMBER);
        CHECK(lua_tointeger(L, -1) == 2);
    }
    SUBCASE("By move")
    {
        auto rLen = luaL_len(L, LUA_REGISTRYINDEX);

        luaL_loadstring(L, "i = 1; i = i + 1");
        Script::LuaFunction s(L, -1);
        lua_pop(L, 1);

        // An entry on the registry should be created
        CHECK(rLen < luaL_len(L, LUA_REGISTRYINDEX));
        rLen = luaL_len(L, LUA_REGISTRYINDEX);
        
        Script::LuaFunction t = std::move(s);
        // No entry is created this time
        CHECK(rLen == luaL_len(L, LUA_REGISTRYINDEX));

        t.PushFunction();
        // T holds a function...
        CHECK(lua_type(L, -1) == LUA_TFUNCTION);

        s.PushFunction();
        // ... s holds nothing
        CHECK(lua_type(L, -1) == LUA_TNIL);
    }
    SUBCASE("Move assignment")
    {
        auto rLen = luaL_len(L, LUA_REGISTRYINDEX);

        luaL_loadstring(L, "i = 1; i = i + 1");
        Script::LuaFunction f(L, -1);
        lua_pop(L, 1);

        // An entry on the registry should be created for f
        CHECK(rLen < luaL_len(L, LUA_REGISTRYINDEX));
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        luaL_loadstring(L, "j = 1; j = j + 1");
        Script::LuaFunction g(L, -1);
        lua_pop(L, 1);

        // An entry on the registry should be created for g
        CHECK(rLen < luaL_len(L, LUA_REGISTRYINDEX));
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        g = std::move(f);

        // Now g points to the function that was held in f
        // The function held by g was destroyed
        CHECK(luaL_len(L, LUA_REGISTRYINDEX) < rLen);

        g.PushFunction();
        // g holds a function...
        CHECK(lua_type(L, -1) == LUA_TFUNCTION);

        f.PushFunction();
        // ... f holds nothing
        CHECK(lua_type(L, -1) == LUA_TNIL);
    }

    // Check there are no objects left on the registry
    lua_pushinteger(L, 1);
    CHECK(luaL_ref(L, LUA_REGISTRYINDEX) == (baseLen + 1));

    lua_close(L);
}