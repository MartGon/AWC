#include <doctest.h>

#include <AWC/AWCusing.h>
#include <AWC/UnitType.h>
#include <AWC/Unit.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/CostTable.h>
#include <AWC/MovementDescType.h>
#include <AWC/WeaponType.h>

#include <UnitTest.h>

UnitType UnitTest::CreateSoldierType()
{
    // Name and id
    uint id = 0;
    std::string name = "Soldier";

    // TilePatternDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    TilePatternDescIPtr manhattan = TilePatternDesc::Create(manhattanMoves);

    // CostTables
    std::shared_ptr<CostTable> unitCostTable{new CostTable};
    unitCostTable->SetCost(0, 1);

    CostTablePtr tileCostTable{new CostTable};
    uint grassId = 0;
    tileCostTable->SetCost(grassId, 1);

    // Movement
    MovementDecTypePtr moveType{ new MovementDescType{manhattan, {3, 0}, tileCostTable, unitCostTable, 99}};

    // Weapon
    AttackTable attackTable{ {{id, true}} };
    WeaponTypePtr weaponType{ new WeaponType{manhattan, {1, 1}, attackTable, 99, 10}};

    UnitType soldierType{id, "Soldier", moveType, weaponType};

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