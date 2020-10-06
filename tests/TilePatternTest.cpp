#include "doctest.h"

#include <memory>

#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePattern.h>
#include <AWC/Map.h>

TEST_CASE("Manhattan TilePattern test")
{
    // Construct manhattan tile Pattern
    std::vector<Vector2> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::unordered_map<Vector2, std::vector<Vector2>> excludedDirections = 
    {
        {{1, 0}, {{1, 0}}},
        {{0, 1}, {{0, 1}}},
        {{-1, 0}, {{-1, 0}}},
        {{0, -1}, {{0, -1}}}
    };
    TilePatternDescriptor manhattanTilePattern{directions, excludedDirections};

    // Construct
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    SUBCASE("Test constructors")
    {
        TilePatternDescriptor manhattanTilePatternImplicit{directions};
        std::vector<Vector2> rightLockedDirections = {{0, 1}, {-1, 0}, {0, -1}};
        std::vector<Vector2> leftLockedDirections = {{1, 0}, {0, 1}, {0, -1}};
        std::vector<Vector2> upLockedDirections = {{1, 0}, {-1, 0}, {0, -1}};
        std::vector<Vector2> downLockedDirections = {{1, 0}, {0, 1}, {-1, 0}};

        // Implicit
        CHECK(manhattanTilePatternImplicit.GetLockedDirections({1, 0}) == rightLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections({-1, 0}) == leftLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections({0, 1}) == upLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections({0, -1}) == downLockedDirections);

        // Explicit
        auto rightLockedDirectionsTest = manhattanTilePattern.GetLockedDirections({1, 0});
        auto leftLockedDirectionsTest = manhattanTilePattern.GetLockedDirections({-1, 0});
        auto upLockedDirectionsTest = manhattanTilePattern.GetLockedDirections({0, 1});
        auto downLockedDirectionsTest = manhattanTilePattern.GetLockedDirections({0, -1});

        CHECK(rightLockedDirectionsTest == rightLockedDirections);
        CHECK(leftLockedDirectionsTest == leftLockedDirections);
        CHECK(upLockedDirectionsTest == upLockedDirections);
        CHECK(downLockedDirectionsTest == downLockedDirections);
        
    }
    SUBCASE("Should calculate if a tile is in Range")
    {
        /*
        auto tilePattern = manhattanTilePattern.CalculateTilePattern({0, 0}, 5);

        CHECK(tilePattern->IsTileInRange({0, 0}) == true);
        CHECK(tilePattern->IsTileInRange({1, 0}) == true);
        CHECK(tilePattern->IsTileInRange({1, 1}) == true);
        CHECK(tilePattern->IsTileInRange({2, 3}) == true);
        CHECK(tilePattern->IsTileInRange({3, 2}) == true);

        CHECK(tilePattern->IsTileInRange({9, 9}) == false);
        CHECK(tilePattern->IsTileInRange({7, 3}) == false);
        CHECK(tilePattern->IsTileInRange({-1, 0}) == false);
        CHECK(tilePattern->IsTileInRange({0, -1}) == false);
        */
    }
}