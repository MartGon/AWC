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

Event::Listener GetNilListener(Event::HandlerCallback cb)
{
    auto entType = Entity::Type::UNIT;
    Entity::Entity ent{ entType, UnitNS::GUID(0, 0)};

    auto opType = Operation::Type::CUSTOM;
    Event::Handler handler{opType, cb};

    Event::Listener listener{ent, handler};

    return listener;
}

TEST_CASE("Event::Subject::Add/Remove/Iteration test")
{
    Game game;

    SUBCASE("Modifying the list while iterating through it")
    {
        int counter = 0;
        int counterObjective = 5;

        Event::HandlerCallback recursiveCB = [&recursiveCB, &counter, counterObjective]
        (Event::Notification::Notification noti, Entity::Entity ent, Game& game)
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

        CommandPtr null{ new NullCommand{}};
        game.ExecuteCommand(null, 0);

        CHECK(counter == 1);

        game.Push(custom);
        game.ExecuteCommand(null, 0);

        // On Pre only one is listening, so counter == 1
        // On Post, two of them are listening so counter = counter + 2; counter == 3
        CHECK(counter == 3);
    }
    SUBCASE("Listener removal")
    {   
        int count = 0;
        Event::HandlerCallback cb = [&count](Event::Notification::Notification noti, Entity::Entity ent, Game& game)
        {
            if(noti.type == Event::Notification::Type::PRE)
                count++;
        };
        auto listener = GetNilListener(cb);
        auto& subject = game.GetSubject();
        subject.Register(listener);

        OperationIPtr custom{ new Operation::Custom([&count](Game& game){
            
        })};
        game.Push(custom);

        CommandPtr null{new NullCommand};
        game.ExecuteCommand(null, 0);

        // Is called here
        CHECK(count == 1);

        subject.Unregister(listener.entity, listener.handler.type);
        game.Push(custom);
        game.ExecuteCommand(null, 0);

        // Is not called after being unregistered
        CHECK(count == 1);
    }
}

TEST_CASE("Event::Subject listen by notification")
{
    using namespace Event;

    Game game;
    Event::Subject& sub = game.GetSubject();

    Notification::Type first = Notification::Type::NONE;

    auto countPre = 0;
    auto cbPre = [&countPre, &first](Notification::Notification noti, Entity::Entity e, Game& game)
    {
        countPre++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::PRE;
    };

    auto countPost = 0;
    auto cbPost = [&countPost, &first](Notification::Notification noti, Entity::Entity e, Game& game)
    {
        countPost++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::POST;
    };

    auto counterAny = 0;
    auto cbAny = [&counterAny, &first](Notification::Notification noti, Entity::Entity e, Game& game)
    {
        counterAny++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::ANY;
    };

    sub.Register(Operation::Type::CUSTOM, cbPre, Notification::Type::PRE);
    sub.Register(Operation::Type::CUSTOM, cbPost, Notification::Type::POST);
    sub.Register(Operation::Type::CUSTOM, cbAny, Notification::Type::ANY);

    OperationIPtr custom{ new Operation::Custom([](Game& game){
            
        })};
    game.Push(custom);

    CommandPtr null{new NullCommand};
    game.ExecuteCommand(null, 0);

    CHECK(countPre == 1);
    CHECK(countPost == 1);
    CHECK(counterAny == 2);
    CHECK(first == Notification::Type::PRE);
}