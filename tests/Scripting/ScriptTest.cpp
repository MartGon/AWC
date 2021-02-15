#include "doctest.h"

#include <lua.hpp>

#include <string>

TEST_CASE("Script test")
{
    bool alright = false;
    auto luaState = luaL_newstate();

    std::string testFile = std::string(SCRIPTS_DIR) + "test.lua";
    auto res = luaL_loadfile(luaState, testFile.c_str());

    if(res == LUA_OK)
    {
        if(LUA_OK == lua_pcall(luaState, 0, 0, 0))
        {
            auto type = lua_getglobal(luaState, "TEST");
            int value = lua_tointeger(luaState, -1);
            alright = value == 3;
        }
    }

    lua_close(luaState);

    CHECK(alright == true);
}