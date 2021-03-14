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
        SUBCASE("UserData Copy")
        {
            Script::UserData::Init(luaState);

            lt.SetDataCopy<Script::UserData::Vector2>("vec", Vector2{0, 1});

            auto vec = *lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vec == Vector2{0, 1});
        }
        SUBCASE("UserData Ref")
        {
            // This can't be done with a Vector2 beacause its metatable has a __gc method
            // To delete the copies. Some trick has to be done if every UserData can be used as a ref and copy.
            Script::UserData::Init(luaState);

            Vector2 vec{3, 3};

            lt.SetDataRef<Script::UserData::Vector2>("vec", &vec);
            auto vecRef = lt.GetUserData<Script::UserData::Vector2>("vec");

            CHECK(vecRef == &vec);
        }

        CHECK(lua_gettop(luaState) == 0);
    }

    lua_close(luaState);
}