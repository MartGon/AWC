#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("MovementDescType userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("New")
    {
        std::string path = Test::Script::GetUserDataPath() + "/MovementDescType/New.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushDebugScript(t.ref);
        game.Run();
        
        auto mdt = *sTable.GetUserData<Script::UserData::MovementDescType>("mdt");
        CHECK(mdt->maxGas == -1);
        CHECK(mdt->range.minRange == 1);
        CHECK(mdt->range.maxRange == -1);
        CHECK(mdt->tileCostTable.GetCost(1) == 1);
        CHECK(mdt->unitCostTable.GetCost(1) == -1);
        
    }
}