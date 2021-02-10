#include "doctest.h"

#include <UnitTest.h>

#include <AWC/TileType.h>
#include <AWC/Game.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Map.h>
#include <AWC/Command.h>
#include <AWC/Event.h>
#include <AWC/Operation/Custom.h>

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
    
    bool check = false;
    auto callback = [&check](Event::Notification::Notification noti, Entity::Entity entity, Game& game)
    {
        if(noti.type == Event::Notification::Type::POST){
            check = true;
            std::cout << "I see you movin' !!!\n";

            auto unitGUID = entity.guid.unitGUID;
            auto pos = game.GetUnitPos(unitGUID);
            auto unit = game.GetUnit(unitGUID);
            std::cout << "My id is " << unitGUID.ToString() << '\n';
            std::cout << "I'm at pos " << pos->pos.ToString() << '\n';
            std::cout << "My name is " << unit->GetName() << '\n';
        }
    };

    Event::Handler handler{Operation::Type::MOVE, callback};
    handler.type = Operation::Type::MOVE;

    soldierType.AddHandler(handler);
    auto soldierTwo = soldierType.CreateUnit(playerOne);
    auto soldierOne = soldierType.CreateUnit(playerOne);
    auto& events = game.GetSubject();
    soldierOne->RegisterHandlers(events);

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

TEST_CASE("Event::Subjec test")
{
    Game game;
    int counter = 0;
    int counterObjective = 5;
    Event::HandlerCallback recursiveCB = [&recursiveCB, &counter, counterObjective](Event::Notification::Notification noti, Entity::Entity ent, Game& game)
    {
        if(noti.type == Event::Notification::Type::PRE)
        {
            std::cout << "Handling custom operation with counter " << counter << "\n";
            if(counter < counterObjective)
            {
                auto& subject = game.GetSubject();
                subject.Register(Operation::Type::CUSTOM, recursiveCB);
                counter++;
            }
        }
    };
    auto& subject = game.GetSubject();
    subject.Register(Operation::Type::CUSTOM, recursiveCB);

    OperationIPtr custom{ new Operation::Custom([](Game& game){
        std::cout << "Custom operation this is \n";
    })};
    game.Push(custom);

    SUBCASE("Modifying the list while iterating through it")
    {
        CommandPtr null{ new NullCommand{}};
        game.ExecuteCommand(null, 0);

        CHECK(counter == counterObjective);
    }
}