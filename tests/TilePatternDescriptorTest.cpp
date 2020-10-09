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

    SUBCASE("Check constructing without excluding")
    {
        auto manhattanTilePatternImplicit = TilePatternDescriptor::CreateTilePatternDescriptor(directions);

        // Implicit
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanTilePatternImplicit.GetLockedDirections(s) == southLockedDirections);
    }
    SUBCASE("Check constructing by excluding")
    {
        std::unordered_map<Vector2, std::vector<Vector2>> exclusiveDirections = 
        {
            {e, {w}},
            {w, {e}},
            {n, {s}},
            {s, {n}}
        };
        auto manhattanTilePattern = TilePatternDescriptor::CreateTilePatternDescriptorByExclusiveDirectionsMap(directions, exclusiveDirections);

        CHECK(manhattanTilePattern.GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(s) == southLockedDirections);
    }
    SUBCASE("Check constructing by locking")
    {
        std::unordered_map<Vector2, std::vector<Vector2>> exclusiveDirections = 
        {
            {e, eastLockedDirections},
            {w, westLockedDirections},
            {n, northLockedDirections},
            {s, southLockedDirections}
        };
        auto manhattanTilePattern = TilePatternDescriptor::CreateTilePatternDescriptorByLockedDirectionsMap(directions, exclusiveDirections);

        CHECK(manhattanTilePattern.GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanTilePattern.GetLockedDirections(s) == southLockedDirections);
    }
}

TEST_CASE("Configuration interface tests")
{
    // Construct manhattan tile Pattern
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, w, n, s};

    auto manhattanTilePattern = TilePatternDescriptor::CreateTilePatternDescriptor(directions);

    SUBCASE("Check configuration of directions")
    {
        CHECK(manhattanTilePattern.IsDirection(e) == true);
        CHECK(manhattanTilePattern.IsDirection({1, 1}) == false);

        CHECK_THROWS_AS(manhattanTilePattern.RemoveDirection({1, 1}), const TilePatternDescriptorNoExistingDirection&);
        CHECK_NOTHROW(manhattanTilePattern.RemoveDirection(e));

        CHECK_THROWS_AS(manhattanTilePattern.AddDirection(n), const TilePatternDescriptorAlreadyExistingDirection&);
        CHECK_NOTHROW(manhattanTilePattern.AddDirection({1, 1}));
    }

    SUBCASE("Check configuration of locked directions")
    {
        manhattanTilePattern.SetLockedDirections(n, {e, w});

        CHECK(manhattanTilePattern.GetLockedDirections(n) == std::vector{e, w});

        manhattanTilePattern.SetExclusiveDirections(n, {n, s});

        CHECK(manhattanTilePattern.GetLockedDirections(n) == std::vector{e, w});
    }
}