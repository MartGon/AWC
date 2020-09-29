#include "doctest.h"

#include <memory>

#include <AWC/TilePattern.h>
#include <AWC/Map.h>

TEST_CASE("TilePatterns test")
{
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};
    auto tilePattern = std::make_unique<ManhatanPattern>(0, 2);

    SUBCASE("Check movement")
    {
        // TODO: Reactivate this tests when needed.
        /*
        CHECK(tilePattern->IsTileInRange(map, 0, 0, 1, 1) == true);
        CHECK(tilePattern->IsTileInRange(map, 0, 0, 2, 2) == false);
        CHECK(tilePattern->IsTileInRange(map, 0, 0, 1, 0) == true);
        */
    }
}