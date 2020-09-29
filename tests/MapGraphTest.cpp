#include "doctest.h"

#include <AWC/MapGraph.h>

TEST_CASE("MapGraph test")
{
    MapGraph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1);
    auto neiLeft = mg.CreateNode({-1, 0}, 1);
    auto neiUp = mg.CreateNode({0, 1}, 1);
    auto neiDown = mg.CreateNode({0, -1}, 1);

    
    origin.lock()->AddNeigbour(neiRight.lock()->pos, neiRight);
    origin.lock()->AddNeigbour(neiLeft.lock()->pos, neiLeft);
    origin.lock()->AddNeigbour(neiUp.lock()->pos, neiUp);
    origin.lock()->AddNeigbour(neiDown.lock()->pos, neiDown);

    neiRight.lock()->AddNeigbour(origin.lock()->pos, origin);
    neiUp.lock()->AddNeigbour(origin.lock()->pos, origin);
    neiLeft.lock()->AddNeigbour(origin.lock()->pos, origin);
    neiDown.lock()->AddNeigbour(origin.lock()->pos, origin);
    
    SUBCASE("Smart pointer test")
    {
        CHECK(origin.use_count() == 1);
        CHECK(origin.expired() == false);
    }
    SUBCASE("Created Nodes always exist")
    {
        CHECK(mg.NodeExists(origin.lock()->pos) == true);
        CHECK(mg.NodeExists(neiLeft.lock()->pos) == true);
        CHECK(mg.NodeExists(neiRight.lock()->pos) == true);
        CHECK(mg.NodeExists(neiUp.lock()->pos) == true);
        CHECK(mg.NodeExists(neiDown.lock()->pos) == true);
    }
}