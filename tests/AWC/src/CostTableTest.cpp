#include "doctest.h"

#include <AWC/CostTable.h>

// Size checking
TEST_CASE("Cost table")
{
    CostTable costTable;

    auto grassEntry = std::pair<uint, uint>{1, 1};
    auto waterEntry = std::pair<uint, uint>{2, 4};

    costTable.SetCost(grassEntry.first, grassEntry.second);
    costTable.SetCost(waterEntry.first, waterEntry.second);

    SUBCASE("Retrieve correct cost")
    {
        CHECK(costTable.GetCost(grassEntry.first) == grassEntry.second);
        CHECK(costTable.GetCost(waterEntry.first) == waterEntry.second);
    }
}

TEST_CASE("Fixed CostTable")
{
    const uint cost = 1;
    FixedCostTable costTable{1};

    const uint GRASS_ID = 1;
    const uint WATER_ID = 2;

    SUBCASE("Retrieve always Fixed cost")
    {
        CHECK(costTable.GetCost(GRASS_ID) == 1);
        CHECK(costTable.GetCost(WATER_ID) == 1);
    }
}