#include "doctest.h"

#include <UnitTest.h>

#include <AWC/TileType.h>
#include <AWC/Game.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Map.h>
#include <AWC/Command.h>

#include <iostream>


TEST_CASE("Event test")
{
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

    // Create soldier
    UnitType soldierType = UnitTest::CreateSoldierType();

    
    Event::Listener listener;
    listener.type = Operation::Type::MOVE;
    bool check = false;
    listener.handler = [&check](Process p, Event::NotificationType type)
    {
        if(type == Event::NotificationType::POST){
            check = true;
            std::cout << "I see you movin' !!!\n";
        }
    };

    soldierType.AddListener(listener);
    auto soldierOne = soldierType.CreateUnit(playerOne);
    auto& events = game.GetSubject();
    soldierOne->RegisterListeners(events);

    // Add to map
    map.AddUnit({0, 0}, soldierOne);
    game.AddMap(map);

    CommandPtr moveCommand{new MoveCommand(0, {0, 0}, {1, 0})};

    SUBCASE("Check listener has been executed")
    {
        CHECK(check == false);

        game.ExecuteCommand(moveCommand);

        CHECK(check == true);
    }
    
}