#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("Lua Table access test")
{
    auto luaState = luaL_newstate();

    // Scope so lt is destroyed before luaState
    {
        Script::LuaTable<Script::Scope::External> lt{luaState};

        SUBCASE("Integers")
        {
            lt.Set("Int", 1);

            CHECK(lua_gettop(luaState) == 0);

            auto var = lt.Get<int>("Int");
            CHECK(var == 1);
        }
        SUBCASE("Strings")
        {
            lt.Set<std::string>("String", "str");

            CHECK(lua_gettop(luaState) == 0);

            auto var = lt.Get<std::string>("String");
            CHECK(var == "str");
        }
        SUBCASE("Bools")
        {
            lt.Set("false", false);
            lt.Set("true", true);

            auto f = lt.Get<bool>("false");
            auto t = lt.Get<bool>("true");

            CHECK(f == false);
            CHECK(t == true);
        }
        SUBCASE("UserData Copy")
        {
            Script::UserData::Init(luaState);

            Vector2 vec{0, 1};
            lt.SetDataCopy<Script::UserData::Vector2>("vec", vec);

            vec.x = 1;

            auto vecCopy = *lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vecCopy.x == 0);
            CHECK(Vector2{0, 1} == vecCopy);
        }
        SUBCASE("UserData Ref")
        {
            Script::UserData::Init(luaState);

            Vector2 vec{3, 3};
            vec.x = 1;

            lt.SetDataRef<Script::UserData::Vector2>("vec", &vec);
            auto vecRef = lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vecRef == &vec);
            CHECK(vecRef->x == 1);
        }
        SUBCASE("Inner table")
        {
            Script::LuaTable<Script::Scope::External> table{luaState};
            table.Set("int", 1);
            table.Set("str", "text");

            lt.SetLuaWrapper<Script::LuaTable<Script::Scope::External>>("table", table);

            auto innerTable = lt.GetLuaWrapper<Script::LuaTable<Script::Scope::External>>("table");

            CHECK(innerTable->Get<int>("int") == 1);
            CHECK(innerTable->Get<std::string>("str") == "text");
        }
        SUBCASE("Inner Function")
        {
            luaL_loadstring(luaState, "i = 2;");
            Script::LuaFunction<Script::Scope::External> f{luaState, -1};
            lua_pop(luaState, 1);

            lt.SetLuaWrapper<Script::LuaFunction<Script::Scope::External>>("f", f);

            auto innerFunction = lt.GetLuaWrapper<Script::LuaFunction<Script::Scope::External>>("f");
            CHECK(innerFunction.has_value() == true);

            innerFunction->PushInternal();
            lua_pcall(luaState, 0, 0, 0);
            
            lua_getglobal(luaState, "i");
            CHECK(lua_tointeger(luaState, -1) == 2);

            lua_pop(luaState, 1);
        }
        SUBCASE("Get type")
        {
            lt.Set("int", 1);
            lt.Set("str", "text");
            lt.Set(1, true);

            CHECK(lt.GetType("int") == LUA_TNUMBER);
            CHECK(lt.GetType("str") == LUA_TSTRING);
            CHECK(lt.GetType(1) == LUA_TBOOLEAN);
        }
        SUBCASE("Contains")
        {
            lt.Set("int", 1);
            lt.Set(1, 2);

            CHECK(lt.ContainsValue("int") == true);
            CHECK(lt.ContainsValue(1) == true);
        }
        SUBCASE("Length")
        {
            lt.Set(1, "first");

            CHECK(lt.Length() == 1);

            lt.Set(3, "third");

            CHECK(lt.Length() == 1);

            lt.Set(2, "second");

            CHECK(lt.Length() == 3);
        }
        SUBCASE("Metatable")
        {
            luaL_newmetatable(luaState, "Metatable");
            lua_pop(luaState, 1);

            lt.SetMetaTable("Metatable");
            std::string mtName = lt.GetMetaTableName();

            CHECK(mtName == "Metatable");
        }
        SUBCASE("Push Lua table")
        {
            lt.PushInternal();

            CHECK(lua_gettop(luaState) == 1);
            CHECK(lua_type(luaState, -1) == LUA_TTABLE);

            lua_pop(luaState, 1);
        }

        CHECK(lua_gettop(luaState) == 0);
    }

    lua_close(luaState);
}

TEST_CASE("Creation test")
{
    auto luaState = luaL_newstate();
    auto baseLen = luaL_len(luaState, LUA_REGISTRYINDEX);

    SUBCASE("Create new table")
    {
        Script::LuaTable<Script::Scope::External> lt{luaState};
    }
    SUBCASE("Create new table with metatable")
    {
        luaL_newmetatable(luaState, "Metatable");
        lua_pop(luaState, 1);
        Script::LuaTable<Script::Scope::External> lt(luaState, "Metatable");

        lt.PushInternal();
        CHECK(lua_getmetatable(luaState, 1) == 1);
        lua_getfield(luaState, -1, "__name");
        std::string s{lua_tostring(luaState, -1)};

        CHECK(s == "Metatable");
    }
    SUBCASE("Create from existing table at the stack")
    {
        // Nothing at top
        CHECK_THROWS_AS(Script::LuaTable<Script::Scope::External>(luaState, -1), const AWC::Exception&);
        CHECK(lua_gettop(luaState) == 0);

        // No valid type
        lua_pushinteger(luaState, 1);
        CHECK_THROWS_AS(Script::LuaTable<Script::Scope::External>(luaState, -1), const AWC::Exception&);
        lua_pop(luaState, 1);
        CHECK(lua_gettop(luaState) == 0);

        // valid
        lua_newtable(luaState);
        lua_pushinteger(luaState, 15);
        lua_setfield(luaState, -2, "value");

        Script::LuaTable<Script::Scope::External> lt{luaState, 1};
        CHECK(lt.Get<int>("value") == 15);
    }
    SUBCASE("Create from existing table which is inside another table")
    {
        // Nothing at top
        CHECK_THROWS_AS(Script::LuaTable<Script::Scope::External>(luaState, -1, "value"), const AWC::Exception&);
        CHECK(lua_gettop(luaState) == 0);

        lua_newtable(luaState);
        lua_pushinteger(luaState, 32);
        lua_setfield(luaState, -2, "value");

        // Value at t[n] is not a table
        CHECK_THROWS_AS(Script::LuaTable<Script::Scope::External>(luaState, -1, "value"), const AWC::Exception&);

        lua_pushinteger(luaState, 32);
        lua_setglobal(luaState, "value");
        Script::LuaTable<Script::Scope::External> lt{luaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS};

        CHECK(lt.Get<int>("value") == 32);
    }
    SUBCASE("Create by move")
    {
        auto rLen = luaL_len(luaState, LUA_REGISTRYINDEX);
        Script::LuaTable<Script::Scope::External> lt{luaState};

        // New entry on registry should be created
        CHECK(rLen < luaL_len(luaState, LUA_REGISTRYINDEX));
        rLen = luaL_len(luaState, LUA_REGISTRYINDEX);

        Script::LuaTable<Script::Scope::External> n = std::move(lt);

        // No entry is created
        CHECK(rLen == luaL_len(luaState, LUA_REGISTRYINDEX));

        n.PushInternal();
        CHECK(lua_type(luaState, -1) == LUA_TTABLE);

        lt.PushInternal();
        // Nothing is pushed
        CHECK(lua_type(luaState, -1) == LUA_TNIL);
    }
    SUBCASE("Move assignment operator")
    {
        auto rLen = luaL_len(luaState, LUA_REGISTRYINDEX);
        Script::LuaTable<Script::Scope::External> lt{luaState};

        // New entry on registry should be created
        CHECK(rLen < luaL_len(luaState, LUA_REGISTRYINDEX));
        rLen = luaL_len(luaState, LUA_REGISTRYINDEX);

        Script::LuaTable<Script::Scope::External> a{luaState};
        // Another new entry on registry should be created
        CHECK(rLen < luaL_len(luaState, LUA_REGISTRYINDEX));
        rLen = luaL_len(luaState, LUA_REGISTRYINDEX);

        a = std::move(lt);

        // An entry is removed by move
        CHECK(rLen > luaL_len(luaState, LUA_REGISTRYINDEX));

        a.PushInternal();
        CHECK(lua_type(luaState, -1) == LUA_TTABLE);

        lt.PushInternal();
        // Nothing is pushed
        CHECK(lua_type(luaState, -1) == LUA_TNIL);
    }
    // Check there are no objects left on the registry
    lua_pushinteger(luaState, 1);
    CHECK(luaL_ref(luaState, LUA_REGISTRYINDEX) == (baseLen + 1));

    lua_close(luaState);
}