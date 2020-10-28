#include <doctest.h>

#include <AWC/UnitType.h>
#include <AWC/Unit.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/CostTable.h>

#include <UnitTest.h>

UnitType UnitTest::CreateSoldierType()
{
    // TilePatternDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    TilePatternDescIPtr manhattan = TilePatternDesc::Create(manhattanMoves);

    // CostTables
    std::shared_ptr<CostTable> unitCostTable{new CostTable};
    unitCostTable->SetCost(0, 1);
    CostTableIPtr tileCostTable{new CostTable};

    // Movement
    UnitMovementDesc movementDesc{manhattan, unitCostTable, tileCostTable, 3};

    UnitType soldierType{"Soldier", movementDesc};

    return soldierType;
}

// Size checking
TEST_CASE("Units have some properties")
{
    std::string unitName = "Soldier";
    UnitType soldierType = UnitTest::CreateSoldierType();

    SUBCASE("Units are of a given type")
    {
        auto soldier = soldierType.CreateUnit();
        CHECK(soldier->GetName() == unitName);
    }
}