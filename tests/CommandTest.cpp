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
    auto soldier = soldierType.CreateUnit(0);
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
        auto startingGas = soldier->GetCurrentGas();
        validMoveCommand->Execute(game, 0);

        auto& gameMap  = game.GetMap(0);
        // Unit should now be at (1, 0)
        auto unit = gameMap.GetUnit(1, 0);
        auto currentGas = unit->GetCurrentGas();

        // No unit should be in previous pos
        auto noUnit = gameMap.GetUnit(0, 0);

        CHECK(unit.get() == soldier.get());
        CHECK(noUnit.get() == nullptr);
        CHECK(currentGas < startingGas);
    }
}

TEST_CASE("AttackCommands")
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
    Player playerOne{0, 0, 100};
    Player playerTwo{1, 1, 100};
    game.AddPlayer(playerOne);
    game.AddPlayer(playerTwo);

    UnitType soldierType = UnitTest::CreateSoldierType();
    auto soldierOne = soldierType.CreateUnit(0);
    auto soldierTwo = soldierType.CreateUnit(1);
    auto soldierThree = soldierType.CreateUnit(1);

    map.AddUnit(0, 0, soldierOne);
    map.AddUnit(1, 0, soldierTwo);
    map.AddUnit(9, 9, soldierThree);

    // Add map to game
    game.AddMap(map);

    std::unique_ptr<Command> validAttackCommand = std::make_unique<AttackCommand>(0, Vector2{0, 0}, Vector2{1, 0}, 0);

    SUBCASE("Some of them are valid or not")
    {
        std::unique_ptr<Command> invalidAttackCommand = std::make_unique<AttackCommand>(0, Vector2{-1, -1}, Vector2{1, 0}, 0); // Invalid map origin index
        std::unique_ptr<Command> invalidAttackCommand2 = std::make_unique<AttackCommand>(0, Vector2{0, 0}, Vector2{11, 11}, 0); // Invalid map target index
        CommandPtr invalidAttackCommand3 = std::make_unique<AttackCommand>(0, Vector2{1, 1}, Vector2{1, 0}, 0); // No unit there
        CommandPtr invalidAttackCommand4 = std::make_unique<AttackCommand>(0, Vector2{0, 0}, Vector2{9, 9}, 0); // Unit out of range

        CHECK(validAttackCommand->CanBeExecuted(game, 0) == true);
        CHECK(invalidAttackCommand->CanBeExecuted(game, 0) == false);
        CHECK(invalidAttackCommand2->CanBeExecuted(game, 0) == false);
        CHECK(invalidAttackCommand3->CanBeExecuted(game, 0) == false);
        CHECK(invalidAttackCommand4->CanBeExecuted(game, 0) == false);
    }
    SUBCASE("Valid commands can be executed and yield correct results")
    {
        validAttackCommand->Execute(game, 0);

        auto& gameMap  = game.GetMap(0);

        // Unit at (1, 0) should be damaged
        auto unit = gameMap.GetUnit(1, 0);

        CHECK(unit->GetHealth() < 100);
    }
}