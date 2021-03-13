#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("Lua Table test")
{
    auto luaState = luaL_newstate();

    // Scope so lt is destroyed before luaState
    {
        Script::LuaTable lt{luaState};

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
        SUBCASE("UserData")
        {
            Script::UserData::Init(luaState);

            lt.SetGCData<Script::UserData::Vector2>("vec", Vector2{0, 1});

            auto vec = *lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vec == Vector2{0, 1});
        }
        SUBCASE("Alternative UserData")
        {
            Script::UserData::Init(luaState);

            lt.SetGCData<Script::UserData::Vector2>("vec", Vector2{0, 1});

            auto vec = *lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vec == Vector2{0, 1});
        }

        CHECK(lua_gettop(luaState) == 0);
    }

    lua_close(luaState);
}