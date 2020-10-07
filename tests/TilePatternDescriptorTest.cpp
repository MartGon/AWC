#include "doctest.h"

#include <memory>

#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePattern.h>
#include <AWC/Map.h>

TEST_CASE("Manhattan TilePattern test")
{
    // Construct manhattan tile Pattern
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, w, n, s};

    std::vector<Vector2> eastLockedDirections = {e, n, s};
    std::vector<Vector2> westLockedDirections = {w, n, s};
    std::vector<Vector2> northLockedDirections = {e, w, n};
    std::vector<Vector2> southLockedDirections = {e, w, s};

    // Construct
    int xSize = 10;
    int ySize = 10;
    Map map{xSize, ySize};

    SUBCASE("Check constructing without excluding")
    {
        TilePatternDescriptor manhattanTilePatternImplicit{directions};

        // Implicit
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(s) == southLockedDirections);
    }
    SUBCASE("Check constructing by excluding")
    {
        std::unordered_map<Vector2, std::vector<Vector2>> excludedDirections = 
        {
            {e, {w}},
            {w, {e}},
            {n, {s}},
            {s, {n}}
        };
        TilePatternDescriptor manhattanTilePattern{directions, excludedDirections};

        CHECK(manhattanTilePattern.GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(s) == southLockedDirections);
    }
}