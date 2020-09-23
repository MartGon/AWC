#include "doctest.h"

#include <AWC/Map.h>
#include <AWC/Unit.h>
#include <AWC/Tile.h>

// Size checking
TEST_CASE("Maps have a specific size")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    SUBCASE("They have a given widht and height")
    {
        auto x = map.GetWidht();
        auto y = map.GetHeight();

        CHECK(x == xSize);
        CHECK(y == ySize);
    }
}

// Access
TEST_CASE("Maps hold units in a given position") 
{
    Unit* soldier = new Unit();

    Map map{10, 10};

    int xPos = 0;
    int yPos = 0;
    map.AddUnit(xPos, yPos, soldier);

    SUBCASE("Can be used to retrieve a unit in a given position")
    {
        auto unit = map.GetUnit(0, 0);

        CHECK(soldier == unit);
    }
    SUBCASE("A specific position can be empty")
    {
        auto nullUnit = map.GetUnit(1, 1);

        CHECK(nullUnit == nullptr);
    }
    SUBCASE("Should get the exact unit in a given position")
    {
        Unit* soldier2 = new Unit();
        map.AddUnit(1, 1, soldier2);

        auto unit = map.GetUnit(xPos, yPos);

        CHECK(unit != soldier2);
    }
}

// Edge cases
TEST_CASE("Maps follow some rules")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    Unit* soldier = new Unit;
    map.AddUnit(0, 0, soldier);

    Tile* grass = new Tile;

    SUBCASE("Don't allow operations out of bounds")
    {
        CHECK_THROWS_AS(map.AddUnit(xSize + 1, ySize + 1, soldier), const MapIndexOutOfBounds&);
        CHECK_THROWS_AS(map.SetTile(xSize + 1, ySize + 1, grass), const MapIndexOutOfBounds&);
        CHECK_THROWS_AS(map.GetUnit(-1, -1), const MapIndexOutOfBounds&);
        CHECK_THROWS_AS(map.GetTile(-1, -1), const MapIndexOutOfBounds&);
    }
    SUBCASE("Only one unit can be in a given position")
    {
        Unit* soldier2 = new Unit();
        CHECK_THROWS_AS(map.AddUnit(0, 0, soldier2), const MapInvalidUnitPosition&);
    }
}