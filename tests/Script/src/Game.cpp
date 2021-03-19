#include "doctest.h"

#include <lua.hpp>

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("Config file")
{
    Script::Game sg;
    
    std::string path = Test::Script::GetGameScriptPath("config.lua");
    sg.RunConfig(path);

    auto& vm = sg.GetLuaVM();
    auto& t = vm.GetGlobalTable();

    CHECK(t.Get<int>("value") == 64);
}