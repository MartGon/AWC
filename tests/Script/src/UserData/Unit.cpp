#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

#include <iostream>

TEST_CASE("Unit userdata")
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
    
    SUBCASE("CalculateMovement")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Unit/CalculateMovement.lua";
        Test::Script::TestScript t(path, sGame);
        
        auto L = sGame.GetLuaState();

        auto& sTable = t.lt();
        sTable.SetRawData("map", Script::UserData::Map::MT_NAME, &game.GetMap(0));
        sTable.SetGCData("unit", Script::UserData::Unit::MT_NAME, soldier);
        sTable.SetGCData("origin", Script::UserData::Vector2::MT_NAME, Vector2{2, 0});

        try{        
            sGame.PushScript(t.ref);
            game.Run();
        }
        catch(const std::bad_alloc& e)
        {
            std::cout << e.what() << '\n';
        }

        auto calculated = sTable.Get<bool>("calculated");
        CHECK(calculated == true);
    }
}
