#include "doctest.h"

#include <AWC/Game.h>

#include <AWC/TileType.h>
#include <AWC/UnitType.h>
#include <AWC/Command.h>
#include <AWC/Player.h>

#include <GameTest.h>
#include <UnitTest.h>

TEST_CASE("Game Configuration")
{
    Game game;

    SUBCASE("Map configuration")
    {
        Map mapOne{5, 5};
        Map mapTwo{10, 10};

        game.AddMap(mapOne);
        game.AddMap(mapTwo);

        CHECK(game.GetMapCount() == 2);
        CHECK(game.GetMap(0).GetWidth() == mapOne.GetWidth());
        CHECK(game.GetMap(0).GetHeight() == mapOne.GetHeight());
        CHECK(game.GetMap(1).GetWidth() == mapTwo.GetWidth());
        CHECK(game.GetMap(1).GetHeight() == mapTwo.GetHeight());
        CHECK_THROWS_AS(game.GetMap(2), const std::out_of_range&);

        game.RemoveMap(0);

        CHECK(game.GetMapCount() == 1);
        CHECK(game.GetMap(0).GetWidth() == mapTwo.GetWidth());
        CHECK(game.GetMap(0).GetHeight() == mapTwo.GetHeight());
        CHECK_THROWS_AS(game.GetMap(1), const std::out_of_range&);
    }
    SUBCASE("Player configuration")
    {
        Player playerOne{0, 0, 1000};
        Player playerTwo{1, 1, 1000};

        game.AddPlayer(playerOne);
        game.AddPlayer(playerTwo);

        CHECK(game.GetPlayerCount() == 2);
        CHECK(game.GetPlayer(0).GetId() == playerOne.GetId());
        CHECK(game.GetPlayer(1).GetId() == playerTwo.GetId());
        CHECK_THROWS_AS(game.GetPlayer(2), const std::out_of_range&);

        game.RemovePlayer(1);

        CHECK(game.GetPlayerCount() == 1);
        CHECK(game.GetPlayer(0).GetId() == playerOne.GetId());
        CHECK_THROWS_AS(game.GetPlayer(1), const std::out_of_range&);
    }
}

TEST_CASE("Game State")
{
    Game game = GameTest::PrepareGame();
    game.Start();

    SUBCASE("Turns")
    {
        auto firstTurn = game.GetCurrentTurn();
        game.PassTurn();
        auto secondTurn = game.GetCurrentTurn();
        game.PassTurn();
        auto thirdTurn = game.GetCurrentTurn();

        CHECK(firstTurn.playerIndex == 0);
        CHECK(secondTurn.playerIndex == 1);
        CHECK(firstTurn.playerIndex == 0);
    }
    SUBCASE("Commands: Player One's soldier moves and attacks player's two soldier")
    {   
        // Units and tiles
        // Note: Types must exist, otherwise the references to them break.
        auto soldierType = UnitTest::CreateSoldierType();
        TileType grassType{0, "Grass"}; 

        Player player{0, 0, 1000};
        Player playerTwo{1, 1, 1000};
        auto soldierOne = soldierType.CreateUnit(player);
        auto soldierTwo = soldierType.CreateUnit(playerTwo);
        
        auto& map = game.GetMap(0);

        map.AddUnit({0, 0}, soldierOne);
        map.AddUnit({1, 1}, soldierTwo);

        MapUtils::FillMap(map, grassType);

        // Set of Commands checks
        CommandPtr moveCommand{new MoveCommand{0, Vector2{0, 0}, Vector2{0, 1}}};
        CommandPtr attackCommand{new AttackCommand{0, Vector2{0, 1}, Vector2{1, 1}, 0}};

        CHECK(game.CanExecuteCommand(moveCommand) == true);
        CHECK(game.CanExecuteCommand(attackCommand) == false);

        game.ExecuteCommand(moveCommand);
        
        CHECK(game.CanExecuteCommand(attackCommand) == true);
        CHECK(game.CanExecuteCommand(moveCommand) == false);

        game.ExecuteCommand(attackCommand);

        auto enemySoldier = map.GetUnit({1, 1});
        CHECK(enemySoldier->GetHealth() < 100);

        // Cannot attack again until next player turn
        CHECK(game.CanExecuteCommand(attackCommand) == false);
        
        game.PassTurn();
        game.PassTurn();
        CHECK(game.CanExecuteCommand(attackCommand) == true);
    }
}

Game GameTest::PrepareGame()
{
    Game game;

    // Prepare map
    Map mapOne{5, 5};

    game.AddMap(mapOne);

    // Prepare players
    Player playerOne{0, 0, 1000};
    Player playerTwo{1, 1, 1000};

    game.AddPlayer(playerOne);
    game.AddPlayer(playerTwo);    

    return game;
}