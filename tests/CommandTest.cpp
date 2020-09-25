#include "doctest.h"

#include <memory>

#include <AWC/Command.h>
#include <AWC/Map.h>
#include <AWC/UnitType.h>
#include <AWC/Unit.h>

// Size checking
TEST_CASE("MoveCommands")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    UnitType soldierType{"Soldier"};
    auto soldier = soldierType.CreateUnit();
    map.AddUnit(0, 0, soldier);

    std::unique_ptr<Command> validMoveCommand = std::make_unique<MoveCommand>(map, 0, 0, 1, 0);

    SUBCASE("Some of them are valid or not")
    {
        std::unique_ptr<Command> invalidMoveCommand = std::make_unique<MoveCommand>(map, -1, -1, 1, 0);
        std::unique_ptr<Command> invalidMoveCommand2 = std::make_unique<MoveCommand>(map, 0, 0, 11, 11);

        CHECK(validMoveCommand->CanBeExecuted() == true);
        CHECK(invalidMoveCommand->CanBeExecuted() == false);
        CHECK(invalidMoveCommand2->CanBeExecuted() == false);
    }
    SUBCASE("Valid commands can be executed and yield correct results")
    {
        validMoveCommand->Execute();

        // Unit should now be at (1, 0)
        auto unit = map.GetUnit(1, 0);

        // No unit should be in previous pos
        auto noUnit = map.GetUnit(0, 0);

        CHECK(unit.get() == soldier.get());
        CHECK(noUnit.get() == nullptr);
    }
}