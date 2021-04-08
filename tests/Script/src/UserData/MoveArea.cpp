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
    auto soldier = soldierType.CreateUnit(&p);

    game.AddMap(map);

    game.AddUnit(soldier, {2, 0}, 0);
    
    SUBCASE("CalculateMovement")
    {
        std::string path = Test::Script::GetUserDataPath() + "/MoveArea/CanMove.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();
        auto unitMove = soldier->CalculateMovement(game.GetMap(0), {2, 0});
        
        sTable.SetDataCopy<Script::UserData::MoveArea>("unitMove", unitMove);
        sTable.SetDataCopy<Script::UserData::Vector2>("dest", Vector2{0, 0});

        try{        
            sGame.PushDebugScript(t.ref);
            game.Run();
        }
        catch(const std::bad_alloc& e)
        {
            std::cout << e.what() << '\n';
        }

        auto canMove = sTable.Get<bool>("canMove");
        CHECK(canMove == true);
    }
}
