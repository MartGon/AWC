#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/UserData/UserData.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Map userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    Player p{0, 0, 1000};
    game.AddPlayer(p);

    auto soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit(p);

    game.AddMap(map);

    game.AddUnit(soldier, {0, 0}, 0);

    SUBCASE("GetUnit")
    {
        std::string path = Test::Script::UserData::GetUserDataPath() + "/Map/GetUnit.lua";
        Test::Script::UserData::TestScript t(path, sGame);

        auto& sTable = t.lt();
        sTable.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{2, 0});

        sGame.PushScript(t.ref);
        game.Run();

        auto value = sTable.GetInt("value");
        CHECK(value == 2);

        auto found = sTable.GetInt("found");
        CHECK(found == 0);

        sTable.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});

        sGame.PushScript(t.ref);
        game.Run();
    }
}