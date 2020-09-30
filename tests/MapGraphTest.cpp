#include "doctest.h"

#include <AWC/MapGraph.h>

#include <iostream>

TEST_CASE("MapNode tests")
{
    MapGraph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1);
    auto neiLeft = mg.CreateNode({-1, 0}, 1);
    auto neiUp = mg.CreateNode({0, 1}, 1);
    auto neiDown = mg.CreateNode({0, -1}, 1);

    auto sOrigin = origin.lock();
    sOrigin->AddNeigbour(neiRight.lock()->pos, neiRight);
    sOrigin->AddNeigbour(neiLeft.lock()->pos, neiLeft);
    sOrigin->AddNeigbour(neiUp.lock()->pos, neiUp);
    sOrigin->AddNeigbour(neiDown.lock()->pos, neiDown);

    neiRight.lock()->AddNeigbour(sOrigin->pos, origin);
    neiUp.lock()->AddNeigbour(sOrigin->pos, origin);
    neiLeft.lock()->AddNeigbour(sOrigin->pos, origin);
    neiDown.lock()->AddNeigbour(sOrigin->pos, origin);

    SUBCASE("Smart pointer test")
    {
        CHECK(origin.use_count() == 2);
        CHECK(origin.expired() == false);
    }
    SUBCASE("Check neighbourhood")
    {
        CHECK(sOrigin->GetNeighbours().size() == 4);
        CHECK(sOrigin->GetNeighbour({1, 0}).lock().get() == neiRight.lock().get());
        CHECK(sOrigin->GetNeighbour({-1, 0}).lock().get() == neiLeft.lock().get());
        CHECK(sOrigin->GetNeighbour({0, 1}).lock().get() == neiUp.lock().get());
        CHECK(sOrigin->GetNeighbour({0, -1}).lock().get() == neiDown.lock().get());
    }
    SUBCASE("Check reciprocity")
    {
        CHECK(sOrigin->GetNeighbour({1, 0}).lock()->GetNeighbour(sOrigin->pos).lock().get() == sOrigin.get());
        CHECK(neiRight.lock()->GetNeighbour({0, 0}).lock()->GetNeighbour(neiRight.lock()->pos).lock().get() == neiRight.lock().get());
    }
    SUBCASE("Test exceptions")
    {
        try
        {
            sOrigin->GetNeighbour({-1, -1});
        }
        catch(const MapNodeException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.pos == Vector2{-1, -1});
        }
        
        try
        {
            sOrigin->AddNeigbour({-1, 0}, neiRight);
        }
        catch(const MapNodeException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.pos == Vector2{-1, 0});
        }
    }
}

TEST_CASE("MapGraph test")
{
    MapGraph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1);
    auto neiLeft = mg.CreateNode({-1, 0}, 1);
    auto neiUp = mg.CreateNode({0, 1}, 1);
    auto neiDown = mg.CreateNode({0, -1}, 1);

    auto sOrigin = origin.lock();
    sOrigin->AddNeigbour(neiRight.lock()->pos, neiRight);
    sOrigin->AddNeigbour(neiLeft.lock()->pos, neiLeft);
    sOrigin->AddNeigbour(neiUp.lock()->pos, neiUp);
    sOrigin->AddNeigbour(neiDown.lock()->pos, neiDown);

    neiRight.lock()->AddNeigbour(sOrigin->pos, origin);
    neiUp.lock()->AddNeigbour(sOrigin->pos, origin);
    neiLeft.lock()->AddNeigbour(sOrigin->pos, origin);
    neiDown.lock()->AddNeigbour(sOrigin->pos, origin);
    
    SUBCASE("Created Nodes always exist")
    {
        CHECK(mg.NodeExists(origin.lock()->pos) == true);
        CHECK(mg.NodeExists(neiLeft.lock()->pos) == true);
        CHECK(mg.NodeExists(neiRight.lock()->pos) == true);
        CHECK(mg.NodeExists(neiUp.lock()->pos) == true);
        CHECK(mg.NodeExists(neiDown.lock()->pos) == true);

        CHECK(mg.NodeExists({-1, -1}) == false);
        CHECK(mg.NodeExists({1, -1}) == false);
        CHECK(mg.NodeExists({1, 1}) == false);
        CHECK(mg.NodeExists({-1, -1}) == false);
    }
    SUBCASE("Get Nodes directly from the mapgraph")
    {
        CHECK(mg.GetNode(sOrigin->pos).lock().get() == sOrigin.get());
        CHECK(mg.GetNode(neiRight.lock()->pos).lock().get() == neiRight.lock().get());
    }
    SUBCASE("Test NoExisitngNode exception")
    {
        try
        {
            mg.GetNode({-1, -1});
        }
        catch(const MapGraphException& e)
        {
            std::cout << e.what() << '\n';
            CHECK(e.pos == Vector2{-1, -1});
        }  
    }
    SUBCASE("Test AlreadyExistingNode exception")
    {
        try
        {
            mg.CreateNode({0, 0}, 0);
        }
        catch(const MapGraphException& e)
        {
            std::cout << e.what() << '\n';
            CHECK(e.pos == Vector2{0, 0});
        }  
    }
}
