#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("TilePatternDesc userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    SUBCASE("New")
    {
        std::string path = Test::Script::GetUserDataPath() + "/TilePatternDesc/New.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushScript(t.ref);
        game.Run();
        
        int len = sTable.Length();
        auto num = sTable.GetInt("num");
        
        auto tpd = *sTable.GetUserData<::TilePatternDescPtr>("tp", Script::UserData::TilePatternDesc::MT_NAME);
        std::vector<::Vector2> dirs {::Vector2{0, 1}, ::Vector2{1, 0}};
        auto luaDirs = tpd->GetOriginDirections();
        CHECK(tpd->GetOriginDirections() == dirs);
    }
}