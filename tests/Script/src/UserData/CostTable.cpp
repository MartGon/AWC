#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("CostTable userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("Creatin from lib function")
    {
        std::string file = Test::Script::GetUserDataPath("CostTable/New.lua");
        sGame.RunConfig(file);

        auto& g = sGame.GetLuaVM().GetGlobalTable();
        auto n = g.Get<unsigned int>("ct");
        auto ct = *g.GetUserData<Script::UserData::CostTable>("ct");

        CHECK(ct.GetCost(1) == 13);
        CHECK(ct.GetCost(19) == 1);
    }
    SUBCASE("Creation from table")
    {
        std::string file = Test::Script::GetUserDataPath("CostTable/FromTable.lua");
        sGame.RunConfig(file);

        auto& g = sGame.GetLuaVM().GetGlobalTable();
        auto n = g.Get<unsigned int>("ct");
        auto ct = *g.GetUserData<Script::UserData::CostTable>("ct");

        CHECK(ct.GetCost(1) == 13);
        CHECK(ct.GetCost(19) == 1);
    }
}