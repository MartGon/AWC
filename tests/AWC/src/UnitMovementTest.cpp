#include "doctest.h"

#include <memory>

#include <AWC.h>

#include <Tests/AWC/UnitTest.h>

// Size checking
TEST_CASE("MoveCommands")
{
    // Game
    Game game;

    // Map
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);
    
    // Player
    Player player{0, 0, 100};
    Player enemy{1, 1, 1000};
    game.AddPlayer(player);
    game.AddPlayer(enemy);

    UnitType soldierType = UnitTest::CreateSoldierType();
    UnitType dimRipperType = UnitTest::CreateDimensionalRipper();
    auto soldier = soldierType.CreateUnit(game.GetPlayer(0));
    auto dr = dimRipperType.CreateUnit(game.GetPlayer(0));
    auto enemySoldier = soldierType.CreateUnit(game.GetPlayer(1));
    auto enemySoldier2 = soldierType.CreateUnit(game.GetPlayer(1));
    map.AddUnit(0, 0, soldier);
    map.AddUnit(1, 0, dr);
    map.AddUnit({0, 1}, enemySoldier2);
    map.AddUnit({2, 2}, enemySoldier);

    // Add map to game
    game.AddMap(map);

    auto move = dr->CalculateMovement(map, {1, 0});

    CHECK(move.CanMove({0, 1}) == false);
}
