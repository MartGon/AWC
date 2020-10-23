#include "doctest.h"

#include <memory>

#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePattern.h>
#include <AWC/Map.h>
#include <AWC/AWCException.h>

TEST_CASE("Manhattan TilePattern test")
{
    // Construct manhattan tile Pattern
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    Directions directions = {e, w, n, s};

    Directions eastLockedDirections = {e, n, s};
    Directions westLockedDirections = {w, n, s};
    Directions northLockedDirections = {e, w, n};
    Directions southLockedDirections = {e, w, s};

    SUBCASE("Check constructing without exclusive")
    {
        auto manhattanImplicit = TilePatternDescriptor::Create(directions);

        // Implicit
        CHECK(manhattanImplicit->GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattanImplicit->GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattanImplicit->GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattanImplicit->GetLockedDirections(s) == southLockedDirections);
    }
    SUBCASE("Check constructing by exclusive")
    {
        DirectionsTable exclusiveDirections = 
        {
            {e, {w}},
            {w, {e}},
            {n, {s}},
            {s, {n}}
        };
        auto manhattan = TilePatternDescriptor::CreateByExclusive(directions, exclusiveDirections);

        CHECK(manhattan->GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattan->GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattan->GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattan->GetLockedDirections(s) == southLockedDirections);
    }
    SUBCASE("Check constructing by locking")
    {
        DirectionsTable lockedDirections = 
        {
            {e, eastLockedDirections},
            {w, westLockedDirections},
            {n, northLockedDirections},
            {s, southLockedDirections}
        };
        auto manhattan = TilePatternDescriptor::CreateByLocked(directions, lockedDirections);

        CHECK(manhattan->GetLockedDirections(e) == eastLockedDirections);
        CHECK(manhattan->GetLockedDirections(w) == westLockedDirections);
        CHECK(manhattan->GetLockedDirections(n) == northLockedDirections);
        CHECK(manhattan->GetLockedDirections(s) == southLockedDirections);
    }
}

TEST_CASE("Configuration interface tests")
{
    // Construct manhattan tile Pattern
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    Directions directions = {e, w, n, s};

    auto manhattan = TilePatternDescriptor::Create(directions);

    SUBCASE("Check configuration of directions")
    {
        CHECK(manhattan->IsDirection(e) == true);
        CHECK(manhattan->IsDirection({1, 1}) == false);

        CHECK_THROWS_AS(manhattan->RemoveDirection({1, 1}), const AWCNoExistingIndexException&);
        CHECK_NOTHROW(manhattan->RemoveDirection(e));

        CHECK_THROWS_AS(manhattan->AddDirection(n), const AWCAlreadyExistingIndexException&);
        CHECK_NOTHROW(manhattan->AddDirection({1, 1}));
    }

    SUBCASE("Check configuration of locked directions")
    {
        manhattan->SetLockedDirections(n, {e, w});

        CHECK(manhattan->GetLockedDirections(n) == std::vector{e, w});

        manhattan->SetExclusiveDirections(n, {n, s});

        CHECK(manhattan->GetLockedDirections(n) == std::vector{e, w});
    }
}

TEST_CASE("TilePattern creation tests")
{
    // Construct manhattan tile Pattern
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    Directions directions = {e, w, n, s};

    // Map
    Map map{10, 10};

    auto manhattan = TilePatternDescriptor::Create(directions);
}