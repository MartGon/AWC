#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Vector2 userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("New")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Vector2/New.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        Vector2* origin = sTable.GetFullUserData<::Vector2>("origin", Script::UserData::Vector2::MT_NAME);
        CHECK(*origin == Vector2{0, 0});
    }
    SUBCASE("__add")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Vector2/Add.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        Vector2* sum = sTable.GetFullUserData<::Vector2>("sum", Script::UserData::Vector2::MT_NAME);
        CHECK(*sum == Vector2{1, 1});
    }
    SUBCASE("__sub")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Vector2/Sub.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        Vector2* sub = sTable.GetFullUserData<::Vector2>("sub", Script::UserData::Vector2::MT_NAME);
        CHECK(*sub == Vector2{0, 1});
    }
    SUBCASE("__eq")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Vector2/Eq.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();

        bool eq = sTable.GetBool("eq");
        CHECK(eq == true);
    }
}