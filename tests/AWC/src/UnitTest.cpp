#include <doctest.h>

#include <AWC/AWCusing.h>
#include <AWC/CostTable.h>
#include <AWC/Unit/MovementDescType.h>
#include <AWC/Player.h>
#include <AWC/Area/AreaDesc.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Unit/Unit.h>
#include <AWC/Unit/WeaponType.h>

#include <Tests/AWC/UnitTest.h>

// Size checking
TEST_CASE("Units have some properties")
{
    std::string unitName = "Soldier";
    UnitType soldierType = UnitTest::CreateSoldierType();
    Player player{0, 0, 1000};

    SUBCASE("Units are of a given type")
    {
        auto soldier = soldierType.CreateUnit(&player);
        CHECK(soldier->GetName() == unitName);
    }
}

// Move tests
TEST_CASE("Unit movement test")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    Player player{0, 0, 1000};
    auto soldier = soldierType.CreateUnit(&player);

    SUBCASE("Unit::Move and Unit::GetCurrentGas")
    {
        uint cost = 2;
        uint soldierBaseGas = 99;
        uint startingGas = soldier->GetCurrentGas();

        CHECK(soldier->CanMove() == true);

        soldier->Move(cost);

        CHECK(startingGas == soldierBaseGas);
        CHECK(soldier->GetCurrentGas() == soldierBaseGas - cost);
        CHECK(soldier->CanMove() == false);
    }
}

// Attack tests

TEST_CASE("Unit attack tests")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    UnitType mechType = UnitTest::CreateMechType();
    UnitType fighterType = UnitTest::CreateFighter();
    Player player{0, 0, 1000};
    auto soldier = soldierType.CreateUnit(&player);
    auto mech = mechType.CreateUnit(&player);
    auto fighter = fighterType.CreateUnit(&player);

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

        CHECK(soldier->CanAttack() == false);
        CHECK(mech->CanAttack() == false);
        CHECK(fighter->CanAttack() == false);
    }
}

// Defense tests

TEST_CASE("Unit defense tests")
{
    UnitType soldierType = UnitTest::CreateSoldierType();
    Player player{0, 0, 1000};
    auto soldier = soldierType.CreateUnit(&player);

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
        //CHECK(soldier->IsDead() == true);
    }
}

UnitType UnitTest::CreateSoldierType()
{
    // Name and id
    uint id = 0;
    std::string name = "Soldier";

    // AreaDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    AreaDescIPtr manhattan = AreaDesc::Create(manhattanMoves);

    // CostTables
    CostTable unitCostTable;
    unitCostTable.SetCost(id, std::numeric_limits<uint>::max());

    CostTable tileCostTable;
    uint grassId = 0;
    tileCostTable.SetCost(grassId, 1);

    // Movement
    MovementDescTypePtr moveType{ new MovementDescType{manhattan, {3, 0}, tileCostTable, unitCostTable, 99}};

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

    // AreaDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    AreaDescIPtr manhattan = AreaDesc::Create(manhattanMoves);

    // CostTables
    CostTable unitCostTable;
    unitCostTable.SetCost(0, 0);
    unitCostTable.SetCost(1, 0);

    CostTable tileCostTable;
    uint grassId = 0;
    tileCostTable.SetCost(grassId, 1);

    // Movement
    MovementDescTypePtr moveType{ new MovementDescType{manhattan, {2, 0}, tileCostTable, unitCostTable, 99}};

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

    // AreaDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    AreaDescIPtr manhattan = AreaDesc::Create(manhattanMoves);

    // CostTables
    CostTable unitCostTable;
    unitCostTable.SetCost(0, 0);
    unitCostTable.SetCost(1, 0);
    unitCostTable.SetCost(10, 0);

    CostTable tileCostTable;
    uint grassId = 0;
    tileCostTable.SetCost(grassId, 1);

    // Movement
    MovementDescTypePtr moveType{ new MovementDescType{manhattan, {10, 0}, tileCostTable, unitCostTable, 75}};

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

UnitType UnitTest::CreateDimensionalRipper()
{
    // Name and id
    uint id = 23;
    std::string name = "Fighter";

    // AreaDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    AreaDescIPtr manhattan = AreaDesc::Create(manhattanMoves);

    // CostTables
    CostTable unitCostTable;
    unitCostTable.SetCost(0, std::numeric_limits<uint>::max());
    unitCostTable.SetCost(1, std::numeric_limits<uint>::max());
    unitCostTable.SetCost(10, std::numeric_limits<uint>::max());

    CostTable tileCostTable;
    uint grassId = 0;
    tileCostTable.SetCost(grassId, 1);

    // Movement
    MovementDescTypePtr moveType{ new MovementDescType{manhattan, {(uint)-1, 0}, tileCostTable, unitCostTable, (uint)-1}};

    UnitType soldierType{id, name, moveType, {}};

    return soldierType;
}