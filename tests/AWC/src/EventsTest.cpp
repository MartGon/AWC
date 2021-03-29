#include "doctest.h"

#include <Tests/AWC/UnitTest.h>

#include <AWC/TileType.h>
#include <AWC/Game.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Map.h>
#include <AWC/Command.h>
#include <AWC/Event.h>
#include <AWC/Operation/Custom.h>
#include <AWC/Operation/AntiOperation.h>
#include <AWC/Operation/Operation.h>

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
    game.AddMap(map);

    // Player
    Player playerOne{0, 0, 100};
    Player playerTwo{1, 1, 100};
    game.AddPlayer(playerOne);
    game.AddPlayer(playerTwo);

    // Create soldier
    UnitType soldierType = UnitTest::CreateSoldierType();
    
    auto check = 0;
    auto callback = [&check](Event::Notification::Notification noti, Entity::GUID unitGUID, Game& game)
    {
        if(noti.type == Event::Notification::Type::POST){
            check++;
            std::cout << "I see you movin' !!!\n";

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
    auto soldierOne = soldierType.CreateUnit(playerOne);
    auto soldierTwo = soldierType.CreateUnit(playerOne);

    // Add to map
    game.AddUnit(soldierOne, {0, 0});
    game.AddUnit(soldierTwo, {0, 1});

    CommandPtr moveCommand{new MoveCommand(0, {0, 0}, {1, 0})};
    CommandPtr moveCommand2{new MoveCommand(0, {0, 1}, {1, 1})};

    SUBCASE("Check listener has been executed")
    {
        CHECK(check == 0);

        game.ExecuteCommand(moveCommand);

        // The two soldiers are listening
        CHECK(check == 2);

        game.RemoveUnit({1, 0});
        game.ExecuteCommand(moveCommand2);

        // Now only one is listening
        CHECK(check == 3);
    }
    SUBCASE("Anti Operations cancel current operations")
    {
        auto& subject = game.GetSubject();

        auto cb = [](Event::Notification::Notification noti, Entity::GUID unitGUID, Game& game)
        {
            auto process = noti.process;
            OperationIPtr antiOp{ new Operation::AntiOperation(process.info.id)};
            game.Push(antiOp, process.info.priority + 1);
        };
        subject.Register(Operation::Type::MOVE, cb, Event::Notification::Type::PRE);

        CHECK(check == 0);

        game.ExecuteCommand(moveCommand);

        // Nothing moves
        CHECK(check == 0);
    }
}

Event::Listener GetNilListener(Event::HandlerCallback cb)
{
    auto entType = Entity::Type::UNIT;
    auto opType = Operation::Type::CUSTOM;
    Event::Handler handler{opType, cb};

    Event::Listener listener{Entity::NIL, handler};

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
        (Event::Notification::Notification noti, Entity::GUID ent, Game& game)
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

        OperationIPtr custom{new Operation::Custom([](Game& game){
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
        Event::HandlerCallback cb = [&count](Event::Notification::Notification noti, Entity::GUID, Game&)
        {
            if(noti.type == Event::Notification::Type::PRE)
                count++;
        };
        auto& subject = game.GetSubject();
        auto guid = subject.Register(Operation::Type::CUSTOM, cb);

        OperationIPtr custom{new Operation::Custom([&count](Game& game){
            
        })};
        game.Push(custom);

        CommandPtr null{new NullCommand};
        game.ExecuteCommand(null, 0);

        // Is called here
        CHECK(count == 1);

        subject.Unregister(guid, Operation::Type::CUSTOM);
        game.Push(custom);
        game.ExecuteCommand(null, 0);

        // Is not called after being unregistered
        CHECK(count == 1);

        guid = subject.Register(Operation::Type::CUSTOM, cb);
        game.Push(custom);
        game.ExecuteCommand(null, 0);

        // Counts again
        CHECK(count == 2);

        subject.Unregister(guid);
        game.Push(custom);
        game.ExecuteCommand(null, 0);

        // Stays the same after being removed
        CHECK(count == 2);
    }
}

TEST_CASE("Event::Subject listen by notification")
{
    using namespace Event;

    Game game;
    Event::Subject& sub = game.GetSubject();

    Notification::Type first = Notification::Type::NONE;

    auto countPre = 0;
    auto cbPre = [&countPre, &first](Notification::Notification noti, Entity::GUID e, Game& game)
    {
        countPre++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::PRE;
    };

    auto countPost = 0;
    auto cbPost = [&countPost, &first](Notification::Notification noti, Entity::GUID e, Game& game)
    {
        countPost++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::POST;
    };

    auto counterAny = 0;
    auto cbAny = [&counterAny, &first](Notification::Notification noti, Entity::GUID e, Game& game)
    {
        counterAny++;
        if(first == Notification::Type::NONE)
            first = Notification::Type::ANY;
    };

    sub.Register(Operation::Type::CUSTOM, cbPre, Notification::Type::PRE);
    sub.Register(Operation::Type::CUSTOM, cbPost, Notification::Type::POST);
    sub.Register(Operation::Type::CUSTOM, cbAny, Notification::Type::ANY);

    OperationIPtr custom{new Operation::Custom([](Game& game){
            
        })};
    game.Push(custom);

    CommandPtr null{new NullCommand};
    game.ExecuteCommand(null, 0);

    CHECK(countPre == 1);
    CHECK(countPost == 1);
    CHECK(counterAny == 2);
    CHECK(first == Notification::Type::PRE);
}

TEST_CASE("Sort operations")
{
    Game game;
    auto& sub = game.GetSubject();

    std::vector<int> order{1, 3, 2};
    std::vector<int> check;

    OperationIPtr first{new Operation::Custom([&check](Game& game) {
        check.push_back(1);
    })};

    OperationIPtr second{new Operation::Custom([&check](Game& game) {
        check.push_back(2);
    })};

    OperationIPtr third{new Operation::Custom([&check](Game& game) {
        check.push_back(3);
    })};

    game.Push(third);
    game.Push(first, 255);
    game.Push(second);

    CommandPtr null{new NullCommand};
    
    CHECK(check.empty() == true);

    game.ExecuteCommand(null);

    CHECK(order == check);
}

TEST_CASE("Hugo and large tank")
{
    using namespace Event;
    
    // Game
    Game game;
    std::vector<int> solution{1, 2, 3, 4};
    std::vector<int> orderCheck;

    // Adds map
    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);
    auto mapId = game.AddMap(map);

    // Adds players
    Player playerOne{0, 0, 1000};
    Player playerTwo{1, 1, 1000};
    game.AddPlayer(playerOne);
    game.AddPlayer(playerTwo);

    auto& sub = game.GetSubject();

    // TODO: Need to create each separate unit type

    auto soldierType = UnitTest::CreateSoldierType();

        // Puts units
    auto large = soldierType.CreateUnit(playerOne);
    auto hugo = soldierType.CreateUnit(playerTwo);
    auto cop = soldierType.CreateUnit(playerTwo);
    auto normal = soldierType.CreateUnit(playerOne);

    // Cop stuff

    auto copCB = [&orderCheck](Notification::Notification noti, Entity::GUID e, Game& game)
    {
        auto process = noti.process;
        if(process.op->GetType() == Operation::Type::ATTACK)
        {
            auto attack = process.op->To<Operation::Attack>();
            auto attacker = game.GetMap(attack->origin_.mapIndex).GetUnit(attack->origin_.pos);
            
            auto me = game.GetUnit(e);
            auto myPos = game.GetUnitPos(e);

            auto myGUID = me->GetGUID();
            auto attackerGUID = attacker->GetGUID();
            if(me && myGUID != attackerGUID && myPos.has_value())
            {
                OperationIPtr counterAttack{new Operation::Attack(myPos.value(), attack->origin_, 0)};
                game.Push(counterAttack, process.info.priority + 1);

                orderCheck.push_back(1);
            }
        }
    };
    Handler copHandler{Operation::Type::ATTACK, copCB, Notification::Type::PRE};
    Listener copListener{cop->GetGUID(), copHandler};
    sub.Register(copListener);

    // Hugo stuff
    auto hugoCB = [&orderCheck, &large](Notification::Notification noti, Entity::GUID e, Game& game)
    {
        auto process = noti.process;
        if(process.op->GetType() == Operation::Type::TAKE_DMG)
        {
            auto takeDmg = process.op->To<Operation::TakeDmg>();
            auto victim = takeDmg->victim_;

            if(victim->GetGUID() == large->GetGUID()) // Is Large
            {
                // When large takes damage, we register a callback that negates dmg taken by Hugo
                auto denyDmgCB = [&orderCheck](Notification::Notification noti, Entity::GUID hugo, Game& game)
                {
                    auto process = noti.process;
                    if(process.op->GetType() == Operation::Type::TAKE_DMG)
                    {
                        auto takeDmg = process.op->To<Operation::TakeDmg>();
                        auto victim = takeDmg->victim_;

                        if(victim->GetGUID() == hugo)
                        {
                            OperationIPtr deny{ new Operation::AntiOperation(noti.process.info.id)};
                            game.Push(deny, process.info.priority + 1);

                            orderCheck.push_back(3);
                        }
                    }
                };

                Handler denyDmg{Operation::Type::TAKE_DMG, denyDmgCB, Notification::Type::PRE};
                Listener denyListener{e, denyDmg};
                auto& sub = game.GetSubject();
                sub.Register(denyListener);

                orderCheck.push_back(2);
            }
        }
    };
    Handler hugoHandler{Operation::Type::TAKE_DMG, hugoCB, Notification::Type::POST};
    Listener hugoListener{hugo->GetGUID(), hugoHandler};
    sub.Register(hugoListener);

    // Large stuff
    auto largeCB = [&orderCheck](Notification::Notification noti, Entity::GUID me, Game& game)
    {
        auto process = noti.process;
        if(process.op->GetType() == Operation::Type::ANTI_OPERATION)
        {
            auto antiOp = process.op->To<Operation::AntiOperation>();
            auto deniedOpId = antiOp->targetId_;

            auto deniedProcess = game.GetProcess(deniedOpId);
            if(deniedProcess.has_value() && deniedProcess.value().op->GetType() == Operation::Type::TAKE_DMG)
            {
                auto takeDmg = deniedProcess->op->To<Operation::TakeDmg>();
                auto sourceOp = takeDmg->source_;
                if(sourceOp->GetType() == Operation::Type::ATTACK)
                {
                    auto attack = sourceOp->To<Operation::Attack>();
                    if(attack->attacker_->GetGUID() == me)
                    {
                        OperationIPtr deny{new Operation::AntiOperation(process.info.id)};
                        game.Push(deny, process.info.priority + 1);

                        orderCheck.push_back(4);
                    }
                }
            }
        }
    };
    Handler largeHandler{Operation::Type::ANTI_OPERATION, largeCB, Notification::Type::PRE};
    Listener largeListener{large->GetGUID(), largeHandler};
    sub.Register(largeListener);

    game.AddUnit(large, {0, 0}, mapId);
    game.AddUnit(cop, {0, 1}, mapId);
    game.AddUnit(hugo, {1, 0}, mapId);
    game.AddUnit(normal, {2, 0}, mapId);

    SUBCASE("Cop counter attacks")
    {
        CommandPtr largeAttack{new AttackCommand{mapId, {0, 0}, {1, 0}, 0}};
        
        auto largeHP = large->GetHealth();
        auto hugoHP = hugo->GetHealth();

        game.ExecuteCommand(largeAttack, 0);

        auto hugoLeftHp = hugo->GetHealth();

        CHECK(largeHP > large->GetHealth());
        CHECK(hugoHP > hugoLeftHp);
        CHECK(orderCheck == solution);

        CommandPtr normalAttack{new AttackCommand{mapId, {2, 0}, {1, 0}, 0}};
        game.ExecuteCommand(normalAttack, 0);

        CHECK(hugoLeftHp == hugo->GetHealth());
    }
}