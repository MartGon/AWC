#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
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

    game.AddUnit(soldier, {2, 0}, 0);

    SUBCASE("GetUnit")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Map/GetUnit.lua";
        Test::Script::TestScript t(path, sGame);
        
        auto L = sGame.GetLuaState();

        auto& sTable = t.lt();
        sTable.SetLightUserData("map", Script::UserData::Map::MT_NAME, &game.GetMap(0));
        sTable.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{2, 0});

        auto m = sTable.GetUserData<::Map>("map", Script::UserData::Map::MT_NAME);

        sGame.PushScript(t.ref);
        game.Run();

        auto found = sTable.GetInt("found");
        CHECK(found == 0);

        sTable.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});

        sGame.PushScript(t.ref);
        game.Run();
    }
}