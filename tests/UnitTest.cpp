#include <doctest.h>

#include <AWC/AWCusing.h>
#include <AWC/UnitType.h>
#include <AWC/Unit.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/CostTable.h>
#include <AWC/MovementDescType.h>
#include <AWC/WeaponType.h>

#include <UnitTest.h>

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

// Move tests
TEST_CASE("Unit movement test")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit();

    SUBCASE("Unit::Move and Unit::GetCurrentGas")
    {
        uint cost = 2;
        uint soldierBaseGas = 99;
        uint startingGas = soldier->GetCurrentGas();
        soldier->Move(cost);

        CHECK(startingGas == soldierBaseGas);
        CHECK(soldier->GetCurrentGas() == soldierBaseGas - cost);
    }
}

// Attack tests

TEST_CASE("Unit attack tests")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    UnitType mechType = UnitTest::CreateMechType();
    UnitType fighterType = UnitTest::CreateFighter();
    auto soldier = soldierType.CreateUnit();
    auto mech = mechType.CreateUnit();
    auto fighter = fighterType.CreateUnit();

    uint mainWeapon = 0;
    uint secondWeapon = 1;

    SUBCASE("Unit::GetWeaponCount test")
    {
        CHECK(soldier->GetWeaponCount() == 1);
        CHECK(mech->GetWeaponCount() == 1);
        CHECK(fighter->GetWeaponCount() == 2);
    }
    SUBCASE("Unit::CanAttack test")
    {
        // Soldier
        CHECK(soldier->CanAttack(soldier) == true);
        CHECK(soldier->CanAttack(mech) == true);
        CHECK(soldier->CanAttack(fighter) == false);    
        CHECK(soldier->CanAttackUnitWith(soldier, mainWeapon) == true);
        CHECK(soldier->CanAttackUnitWith(mech, mainWeapon) == true);
        CHECK(soldier->CanAttackUnitWith(fighter, mainWeapon) == false);

        // Mech
        CHECK(mech->GetWeaponCount() == 1);
        CHECK(mech->CanAttack(soldier) == true);
        CHECK(mech->CanAttack(mech) == true);
        CHECK(mech->CanAttack(fighter) == false);    
        CHECK(mech->CanAttackUnitWith(soldier, mainWeapon) == true);
        CHECK(mech->CanAttackUnitWith(mech, mainWeapon) == true);
        CHECK(mech->CanAttackUnitWith(fighter, mainWeapon) == false);

        // Fighter
        CHECK(fighter->GetWeaponCount() == 2);
        CHECK(fighter->CanAttack(soldier) == true);
        CHECK(fighter->CanAttack(mech) == true);
        CHECK(fighter->CanAttack(fighter) == true);    
        CHECK(fighter->CanAttackUnitWith(soldier, mainWeapon) == false);
        CHECK(fighter->CanAttackUnitWith(mech, mainWeapon) == false);
        CHECK(fighter->CanAttackUnitWith(fighter, mainWeapon) == true);
        CHECK(fighter->CanAttackUnitWith(soldier, secondWeapon) == true);
        CHECK(fighter->CanAttackUnitWith(mech, secondWeapon) == true);
        CHECK(fighter->CanAttackUnitWith(fighter, secondWeapon) == false);
    }
    SUBCASE("Unit::GetWeaponDmg test")
    {
        // Soldier
        CHECK(soldier->GetDmgToUnit(mainWeapon, soldier) == 50);
        CHECK(soldier->GetDmgToUnit(mainWeapon, mech) == 25);
        CHECK_THROWS_AS(soldier->GetDmgToUnit(mainWeapon, fighter), const std::out_of_range&);

        // Mech
        CHECK(mech->GetDmgToUnit(mainWeapon, soldier) == 75);
        CHECK(mech->GetDmgToUnit(mainWeapon, mech) == 50);
        CHECK_THROWS_AS(mech->GetDmgToUnit(mainWeapon, fighter), const std::out_of_range&);

        // Fighter weapon 0
        CHECK_THROWS_AS(fighter->GetDmgToUnit(mainWeapon, soldier), const std::out_of_range&);
        CHECK_THROWS_AS(fighter->GetDmgToUnit(mainWeapon, mech), const std::out_of_range&);
        CHECK(fighter->GetDmgToUnit(mainWeapon, fighter) == 50);

        // Fighter weapon 1
        CHECK(fighter->GetDmgToUnit(secondWeapon, soldier) == 75);
        CHECK(fighter->GetDmgToUnit(secondWeapon, mech) == 65);
        CHECK_THROWS_AS(fighter->GetDmgToUnit(secondWeapon, fighter), const std::out_of_range&);
    }
    SUBCASE("Unit::UseWeapon amd GetWeaponAmmo test")
    {
        soldier->UseWeapon(mainWeapon);
        mech->UseWeapon(mainWeapon);
        fighter->UseWeapon(mainWeapon);
        fighter->UseWeapon(secondWeapon);

        CHECK(soldier->GetWeaponAmmo(mainWeapon) == 98);
        CHECK(mech->GetWeaponAmmo(mainWeapon) == 98);
        CHECK(fighter->GetWeaponAmmo(mainWeapon) == 4);
        CHECK(fighter->GetWeaponAmmo(secondWeapon) == 98);
    }
}

// Defense tests

TEST_CASE("Unit defense tests")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit();

    SUBCASE("Unit damage taken test")
    {
        float startingHealth = soldier->GetHealth();

        CHECK(startingHealth == 100);
        float rawDmg = 20;
        soldier->TakeRawDamage(rawDmg);
        CHECK(soldier->GetHealth() == 80);

        float dmgTaken = soldier->GetDmgTaken(rawDmg);
        float expectedHealth = soldier->GetHealth() - dmgTaken;
        soldier->TakeDamage(rawDmg);
        CHECK(soldier->GetHealth() == expectedHealth);
        
        soldier->TakeRawDamage(100);
        CHECK(soldier->GetHealth() <= 0);
        CHECK(soldier->IsDead() == true);
    }
}

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
    unitCostTable->SetCost(0, 0);

    CostTablePtr tileCostTable{new CostTable};
    uint grassId = 0;
    tileCostTable->SetCost(grassId, 1);

    // Movement
    MovementDecTypePtr moveType{ new MovementDescType{manhattan, {3, 0}, tileCostTable, unitCostTable, 99}};

    // Weapon
    AttackTable attackTable{ { {id, true}, {1, true} } };
    DamageTable damageTable{ { {id, 50}, {1, 25} } };
    WeaponTypePtr weaponType{ new WeaponType{manhattan, {1, 1}, attackTable, damageTable, 99}};

    UnitType soldierType{id, name, moveType, {weaponType}};

    return soldierType;
}

UnitType UnitTest::CreateMechType()
{
    // Name and id
    uint id = 1;
    std::string name = "Mech";

    // TilePatternDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    TilePatternDescIPtr manhattan = TilePatternDesc::Create(manhattanMoves);

    // CostTables
    std::shared_ptr<CostTable> unitCostTable{new CostTable};
    unitCostTable->SetCost(0, 0);
    unitCostTable->SetCost(1, 0);

    CostTablePtr tileCostTable{new CostTable};
    uint grassId = 0;
    tileCostTable->SetCost(grassId, 1);

    // Movement
    MovementDecTypePtr moveType{ new MovementDescType{manhattan, {2, 0}, tileCostTable, unitCostTable, 99}};

    // Weapon
    AttackTable attackTable{ { {0, true}, {id, true} } };
    DamageTable damageTable{ { {0, 75}, {id, 50} } };
    WeaponTypePtr weaponType{ new WeaponType{manhattan, {1, 1}, attackTable, damageTable, 99}};

    UnitType soldierType{id, name, moveType, {weaponType}};

    return soldierType;
}

UnitType UnitTest::CreateFighter()
{
    // Name and id
    uint id = 10;
    std::string name = "Fighter";

    // TilePatternDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    TilePatternDescIPtr manhattan = TilePatternDesc::Create(manhattanMoves);

    // CostTables
    std::shared_ptr<CostTable> unitCostTable{new CostTable};
    unitCostTable->SetCost(0, 0);
    unitCostTable->SetCost(1, 0);
    unitCostTable->SetCost(10, 0);

    CostTablePtr tileCostTable{new CostTable};
    uint grassId = 0;
    tileCostTable->SetCost(grassId, 1);

    // Movement
    MovementDecTypePtr moveType{ new MovementDescType{manhattan, {10, 0}, tileCostTable, unitCostTable, 75}};

    // Weapon
    AttackTable attackTable{ { {id, true} } };
    DamageTable damageTable{ { {id, 50} } };
    WeaponTypePtr weaponType{ new WeaponType{manhattan, {1, 1}, attackTable, damageTable, 5}};

    // Second weapon
    AttackTable secAttackTable{ { {0, true}, {1, true} }};
    DamageTable secDamageTable{ { {0, 75}, {1, 65} } };
    WeaponTypePtr secWeaponType{ new WeaponType{manhattan, {1, 1}, secAttackTable, secDamageTable, 99}};

    UnitType soldierType{id, name, moveType, {weaponType, secWeaponType}};

    return soldierType;
}