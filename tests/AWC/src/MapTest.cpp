#include "doctest.h"

#include <memory>

#include <AWC/Map.h>
#include <AWC/Player.h>
#include <AWC/Unit/Unit.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Tile.h>
#include <AWC/TileType.h>
#include <AWC/Exception.h>

#include <Tests/AWC/UnitTest.h>

// Size checking
TEST_CASE("Maps have a specific size")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    SUBCASE("They have a given widht and height")
    {
        auto x = map.GetWidth();
        auto y = map.GetHeight();

        CHECK(x == xSize);
        CHECK(y == ySize);
    }
}

// Access
TEST_CASE("Maps hold units in a given position") 
{
    UnitType soldierUnitType = UnitTest::CreateSoldierType();
    Player player{0, 0, 1000};
    auto soldier = soldierUnitType.CreateUnit(&player);

    Map map{10, 10};

    int xPos = 0;
    int yPos = 0;
    map.AddUnit(xPos, yPos, soldier);

    SUBCASE("Can be used to retrieve a unit in a given position")
    {
        const std::shared_ptr<Unit> unit = map.GetUnit(0, 0);

        CHECK(soldier.get() == unit.get());
    }
    SUBCASE("A specific position can be empty")
    {
        auto nullUnit = map.GetUnit(1, 1);

        CHECK(nullUnit.get() == nullptr);
    }
    SUBCASE("Should get the exact unit in a given position")
    {
        auto soldier2 = soldierUnitType.CreateUnit(&player);
        map.AddUnit(1, 1, soldier2);

        auto unit = map.GetUnit(xPos, yPos);

        CHECK(unit.get() != soldier2.get());
    }
}

TEST_CASE("Maps hold tiles")
{
    Map map{10, 10};

    TileType grassType{0, "Grass"};
    auto grass = grassType.CreateTile();
    map.SetTile(0, 0, grass);

    SUBCASE("Can be use to retrieve a Tile in a given position")
    {
        auto tile = map.GetTile(0, 0);

        CHECK(tile.get() == grass.get());
    }
    SUBCASE("Or set a especific Tile in a given position")
    {
        TileType seaType{1, "Sea"};
        auto sea = seaType.CreateTile();
        map.SetTile(0, 1, sea);

        auto tile = map.GetTile(0, 1);
        CHECK(tile.get() != grass.get());
        CHECK(tile.get() == sea.get());
    }
}

// Edge cases
TEST_CASE("Maps follow some rules")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    UnitType soldierUnitType = UnitTest::CreateSoldierType();
    Player player{0, 0, 1000};
    auto soldier = soldierUnitType.CreateUnit(&player);
    map.AddUnit(0, 0, soldier);

    TileType grassType{0, "Grass"};
    auto grass = grassType.CreateTile();

    SUBCASE("Don't allow operations out of bounds")
    {
        CHECK_THROWS_AS(map.AddUnit(xSize + 1, ySize + 1, soldier), const AWC::NoExistingIndexException&);
        CHECK_THROWS_AS(map.SetTile(xSize + 1, ySize + 1, grass), const AWC::NoExistingIndexException&);
        CHECK_THROWS_AS(map.GetUnit(-1, -1), const AWC::NoExistingIndexException&);
        CHECK_THROWS_AS(map.GetTile(-1, -1), const AWC::NoExistingIndexException&);
    }
    SUBCASE("Only one unit can be in a given position")
    {
        auto soldier2 = soldierUnitType.CreateUnit(&player);
        CHECK_THROWS_AS(map.AddUnit(0, 0, soldier2), const AWC::AlreadyExistingIndexException&);
    }
}