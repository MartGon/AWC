#include "doctest.h"

#include <memory>

#include <AWC/Command.h>
#include <AWC/Game.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/UnitType.h>
#include <AWC/Unit.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/CostTable.h>

#include <UnitTest.h>

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
    game.AddPlayer(player);

    UnitType soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit();
    map.AddUnit(0, 0, soldier);

    // Add map to game
    game.AddMap(map);

    std::unique_ptr<Command> validMoveCommand = std::make_unique<MoveCommand>(0, 0, 0, 1, 0);

    SUBCASE("Some of them are valid or not")
    {
        std::unique_ptr<Command> invalidMoveCommand = std::make_unique<MoveCommand>(0, -1, -1, 1, 0);
        std::unique_ptr<Command> invalidMoveCommand2 = std::make_unique<MoveCommand>(0, 0, 0, 11, 11);

        CHECK(validMoveCommand->CanBeExecuted(game, 0) == true);
        CHECK(invalidMoveCommand->CanBeExecuted(game, 0) == false);
        CHECK(invalidMoveCommand2->CanBeExecuted(game, 0) == false);
    }
    SUBCASE("Valid commands can be executed and yield correct results")
    {
        validMoveCommand->Execute(game, 0);
        
        auto gameMap  = game.GetMap(0);
        // Unit should now be at (1, 0)
        auto unit = gameMap.GetUnit(1, 0);

        // No unit should be in previous pos
        auto noUnit = gameMap.GetUnit(0, 0);

        CHECK(unit.get() == soldier.get());
        CHECK(noUnit.get() == nullptr);
    }
}