#include "doctest.h"

#include <memory>

#include <AWC/Area/AreaDesc.h>
#include <AWC/Area/Area.h>
#include <AWC/Map.h>
#include <AWC/Exception.h>

TEST_CASE("Manhattan Area test")
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
        auto manhattanImplicit = AreaDesc::Create(directions);

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
        auto manhattan = AreaDesc::CreateByExclusive(directions, exclusiveDirections);

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
        auto manhattan = AreaDesc::CreateByLocked(directions, lockedDirections);

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

    auto manhattan = AreaDesc::Create(directions);

    SUBCASE("Check configuration of directions")
    {
        CHECK(manhattan->IsOriginDirection(e) == true);
        CHECK(manhattan->IsOriginDirection({1, 1}) == false);

        CHECK_THROWS_AS(manhattan->RemoveOriginDirection({1, 1}), const AWC::NoExistingIndexException&);
        CHECK_NOTHROW(manhattan->RemoveOriginDirection(e));

        CHECK_THROWS_AS(manhattan->AddOriginDirection(n), const AWC::AlreadyExistingIndexException&);
        CHECK_NOTHROW(manhattan->AddOriginDirection({1, 1}));
    }

    SUBCASE("Check configuration of locked directions")
    {
        manhattan->SetLockedDirections(n, {e, w});

        CHECK(manhattan->GetLockedDirections(n) == std::vector{e, w});

        manhattan->SetExclusiveDirections(n, {n, s});

        CHECK(manhattan->GetLockedDirections(n) == std::vector{e, w});
    }
}