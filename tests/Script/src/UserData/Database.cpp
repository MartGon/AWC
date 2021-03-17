#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Database userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("Get")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Database/Get.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        bool success = sTable.Get<bool>("Success");
        bool successG = sTable.Get<bool>("SuccessG");
        CHECK(success == true);
        CHECK(successG == true);
    }
}