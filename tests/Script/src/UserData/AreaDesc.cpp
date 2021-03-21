#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("AreaDesc userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("New")
    {
        std::string path = Test::Script::GetUserDataPath() + "/AreaDesc/New.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();
        
        int len = sTable.Length();
        auto tpd = *sTable.GetUserData<Script::UserData::AreaDesc>("tp");

        // This is a HACK. It should be tested by calculating a tile pattern
        // This is much easier tho.
        auto ptr = static_cast<AreaDesc*>(tpd.get());
        std::vector<::Vector2> dirs {::Vector2{0, 1}, ::Vector2{1, 0}};
        auto luaDirs = ptr->GetOriginDirections();
        CHECK(ptr->GetOriginDirections() == dirs);
    }
    SUBCASE("New Locked")
    {
        std::string path = Test::Script::GetUserDataPath() + "/AreaDesc/NewLocked.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();
        
        auto tpd = *sTable.GetUserData<Script::UserData::AreaDesc>("tp");

        // This is a HACK. It should be tested by calculating a tile pattern
        // This is much easier tho.
        auto ptr = static_cast<AreaDesc*>(tpd.get());
        std::vector<::Vector2> dirs {::Vector2{0, 1}, ::Vector2{1, 0}};
        auto luaDirs = ptr->GetOriginDirections();
        
        std::vector<Vector2> upLDs = {{0, 1}, {1, 0}, {-1, 0}};
        std::vector<Vector2> rightLDs = {{0, 1}, {1, 0}};

        CHECK(ptr->GetLockedDirections({0, 1}) == upLDs);
        CHECK(ptr->GetLockedDirections({1, 0}) == rightLDs);
        CHECK(ptr->GetOriginDirections() == dirs);
    }
}