#include "doctest.h"

#include <memory>

#include <AWC/Command.h>
#include <AWC/Game.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/UnitType.h>
#include <AWC/Unit.h>
#include <AWC/TilePattern/TilePatternDesc.h>
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
    Player enemy{1, 1, 1000};
    game.AddPlayer(player);
    game.AddPlayer(enemy);

    UnitType soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit(game.GetPlayer(0));
    auto enemySoldier = soldierType.CreateUnit(game.GetPlayer(1));
    auto enemySoldier2 = soldierType.CreateUnit(game.GetPlayer(1));
    map.AddUnit(0, 0, soldier);
    map.AddUnit({0, 1}, enemySoldier2);
    map.AddUnit({2, 2}, enemySoldier);

    // Add map to game
    game.AddMap(map);

    CommandPtr validMoveCommand = std::make_unique<MoveCommand>(0, 0, 0, 1, 0);

    SUBCASE("Some of them are valid or not")
    {
        CommandPtr outOfBounds{new MoveCommand{0, {-1, -1}, {1, 0}}}; // Out of bounds 
        CommandPtr outOfBounds2 {new MoveCommand{0, {0, 0}, {11, 11}}}; // Out of bound2
        CommandPtr noUnit{new MoveCommand{0, {1, 1}, {1, 2}}};
        CommandPtr tooFar{new MoveCommand{0, {0, 0}, {3, 3}}};
        CommandPtr notMyUnit{new MoveCommand{0, {2, 2}, {2, 3}}};
        CommandPtr alreadyAUnit{new MoveCommand{0, {0, 0}, {0, 1}}};

        CHECK(validMoveCommand->CanBeExecuted(game, 0) == true);
        
        CHECK(outOfBounds->CanBeExecuted(game, 0) == false);
        CHECK(outOfBounds2->CanBeExecuted(game, 0) == false);
        CHECK(noUnit->CanBeExecuted(game, 0) == false);
        CHECK(tooFar->CanBeExecuted(game, 0) == false);
        CHECK(notMyUnit->CanBeExecuted(game, 0) == false);
        CHECK(alreadyAUnit->CanBeExecuted(game, 0) == false);

        // Enemy soldier is on the way
        CommandPtr enemyOnTheWay{new MoveCommand{0, {0, 0}, {0, 2}}};
        CHECK(enemyOnTheWay->CanBeExecuted(game, 0) == false); 
    }
    SUBCASE("Valid commands can be executed and yield correct results")
    {
        auto startingGas = soldier->GetCurrentGas();
        validMoveCommand->Execute(game, 0);

        auto& gameMap  = game.GetMap(0);

        // No unit should be in previous pos
        auto noUnit = gameMap.GetUnit(0, 0);

        // Unit should now be at (1, 0)
        auto unit = gameMap.GetUnit(1, 0);
        auto currentGas = unit->GetCurrentGas();

        CHECK(unit.get() == soldier.get());
        CHECK(noUnit.get() == nullptr);
        CHECK(currentGas < startingGas);

        // Cannot move again this turn
        CommandPtr moveAgain{ new MoveCommand{0, {1, 0}, {0, 0}}};
        CHECK(moveAgain->CanBeExecuted(game, 0) == false);
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
    auto soldierOne = soldierType.CreateUnit(game.GetPlayer(0));
    auto friendlySoldier = soldierType.CreateUnit(game.GetPlayer(0));
    auto friendlySoldier2 = soldierType.CreateUnit(game.GetPlayer(0));
    auto soldierTwo = soldierType.CreateUnit(game.GetPlayer(1));
    auto soldierThree = soldierType.CreateUnit(game.GetPlayer(1));

    map.AddUnit(0, 0, soldierOne);
    map.AddUnit(0, 1, friendlySoldier);
    map.AddUnit(1, 1, friendlySoldier2);
    map.AddUnit(1, 0, soldierTwo);
    map.AddUnit(9, 9, soldierThree);

    // Add map to game
    game.AddMap(map);

    CommandPtr validAttackCommand{new AttackCommand(0, Vector2{0, 0}, Vector2{1, 0}, 0)};
    CommandPtr friendlyValidAttack{new AttackCommand{0, Vector2{1, 1}, Vector2{1, 0}, 0}};

    SUBCASE("Some of them are valid or not")
    {
        CommandPtr outOfBounds{new AttackCommand(0, Vector2{-1, -1}, Vector2{1, 0}, 0)}; // Invalid map origin index
        CommandPtr outOfBounds2{new AttackCommand(0, Vector2{0, 0}, Vector2{11, 11}, 0)}; // Invalid map target index
        CommandPtr noUnit{ new AttackCommand(0, Vector2{2, 2}, Vector2{1, 0}, 0)}; // No unit there
        CommandPtr tooFar{new AttackCommand(0, Vector2{0, 0}, Vector2{9, 9}, 0)}; // Unit out of range
        CommandPtr notMyUnit{ new AttackCommand(0, Vector2{1, 0}, Vector2{9, 9}, 0)}; // Not my unit
        CommandPtr notAnEnemy{ new AttackCommand{0, {0, 0}, {0, 1}}};

        CHECK(validAttackCommand->CanBeExecuted(game, 0) == true);
        CHECK(friendlyValidAttack->CanBeExecuted(game, 0) == true);
        CHECK(outOfBounds->CanBeExecuted(game, 0) == false);
        CHECK(outOfBounds2->CanBeExecuted(game, 0) == false);
        CHECK(tooFar->CanBeExecuted(game, 0) == false);
        CHECK(noUnit->CanBeExecuted(game, 0) == false);
        CHECK(notAnEnemy->CanBeExecuted(game, 0) == false);
    }
    SUBCASE("Valid commands can be executed and yield correct results")
    {
        validAttackCommand->Execute(game, 0);

        auto& gameMap  = game.GetMap(0);

        // Unit at (1, 0) should be damaged
        auto victim = gameMap.GetUnit(1, 0);

        CHECK(victim->GetHealth() < 100);

        // Cannot attack again this turn
        CHECK(validAttackCommand->CanBeExecuted(game, 0) == false);

        // Other soldier can still attack
        CHECK(friendlyValidAttack->CanBeExecuted(game, 0) == true);
        friendlyValidAttack->Execute(game, 0);

        // Enemy is now dead
        CHECK(victim->IsDead() == true);
        CHECK(gameMap.GetUnit(1, 0).get() == nullptr);
    }
}