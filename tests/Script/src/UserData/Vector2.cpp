#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/UserData/UserData.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Vector2 userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("New")
    {
        std::string path = Test::Script::UserData::GetUserDataPath() + "/Vector2/New.lua";
        Test::Script::UserData::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        Vector2* origin = sTable.GetFullUserData<::Vector2>("origin", Script::UserData::Vector2::MT_NAME);
        CHECK(*origin == Vector2{0, 0});
    }
}