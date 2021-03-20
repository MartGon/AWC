#include "doctest.h"

#include <AWC.h>
#include <Script.h>

TEST_CASE("Construction")
{
    auto L = luaL_newstate();

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
    SUBCASE("By move")
    {
        auto rLen = luaL_len(L, LUA_REGISTRYINDEX);

        luaL_loadstring(L, "i = 1; i = i + 1");
        Script::LuaFunction s(L, -1);
        lua_pop(L, 1);

        // An entry on the registry should be created
        CHECK(rLen < luaL_len(L, LUA_REGISTRYINDEX));
        rLen = luaL_len(L, LUA_REGISTRYINDEX);

        // It holds a function
        s.PushFunction();
        CHECK(lua_type(L, -1) == LUA_TFUNCTION);
        
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

    lua_close(L);
}