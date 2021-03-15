#include "doctest.h"

#include <AWC/Area/Graph.h>
#include <AWC/AWCException.h>

#include <iostream>

TEST_CASE("Node tests")
{
    Graph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1);
    auto neiLeft = mg.CreateNode({-1, 0}, 1);
    auto neiUp = mg.CreateNode({0, 1}, 1);
    auto neiDown = mg.CreateNode({0, -1}, 1);

    auto sOrigin = origin.lock();
    mg.SetNeighbour(sOrigin->pos, neiRight.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiLeft.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiUp.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiDown.lock()->pos);

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
        catch(const AWCNoExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{-1, -1});
        }
        
        try
        {
            mg.SetNeighbour({-1, 0}, {0, 0});
        }
        catch(const AWCAlreadyExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{0, 0});
        }
    }
}

TEST_CASE("Graph test")
{
    Graph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1);
    auto neiLeft = mg.CreateNode({-1, 0}, 1);
    auto neiUp = mg.CreateNode({0, 1}, 1);
    auto neiDown = mg.CreateNode({0, -1}, 1);

    auto sOrigin = origin.lock();
    mg.SetNeighbour(sOrigin->pos, neiRight.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiLeft.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiUp.lock()->pos);
    mg.SetNeighbour(sOrigin->pos, neiDown.lock()->pos);
    
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
        catch(const AWCNoExistingIndexException& e)
        {
            std::cout << e.what() << '\n';
            CHECK(e.index == Vector2{-1, -1});
        }  
    }
    SUBCASE("Test AlreadyExistingNode exception")
    {
        try
        {
            mg.CreateNode({0, 0}, 0);
        }
        catch(const AWCAlreadyExistingIndexException& e)
        {
            std::cout << e.what() << '\n';
            CHECK(e.index == Vector2{0, 0});
        }  
    }
    SUBCASE("Test SetNeighbour NoExistingNodes exceptions")
    {
        try
        {
            mg.SetNeighbour({-1, -1}, {1, 0});
        }
        catch(const AWCNoExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{-1, -1});
        }

        try
        {
            mg.SetNeighbour({1, 0}, {-1, -1});
        }
        catch(const AWCNoExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{-1, -1});
        }

        try
        {
            mg.SetNeighbour({0, 0}, {1, 0});
        }
        catch(const AWCAlreadyExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{1, 0});
        }   

        try
        {
            mg.SetNeighbour({1, 0}, {0, 0});
        }
        catch(const AWCAlreadyExistingIndexException& e)
        {
            std::cerr << e.what() << '\n';
            CHECK(e.index == Vector2{0, 0});
        }
    }
    SUBCASE("Check move constructor")
    {
        Graph mg;
        Graph mg2{std::move(mg)};
        Graph mg3 = std::move(mg);
#ifdef _DEBUG
        CHECK(mg2.moved == true);
        CHECK(mg3.moved == true);
#endif 
    }
}

TEST_CASE("GetNeigbours by criteria")
{
    Graph mg;
    auto origin = mg.CreateNode({0, 0}, 0);
    auto neiRight = mg.CreateNode({1, 0}, 1, {0, 0});
    auto neiLeft = mg.CreateNode({-1, 0}, 2, {0, 0});
    auto neiUp = mg.CreateNode({0, 1}, 3, {0, 0});
    auto neiDown = mg.CreateNode({0, -1}, 2, {0, 0});

    auto sOrigin = origin.lock();

    SUBCASE("By lowest cost")
    {
        auto lowest = [](std::weak_ptr<Node> a, std::weak_ptr<Node> b) {
            return a.lock()->cost < b.lock()->cost;
        };
        auto match = sOrigin->GetNeighbourBySortCriteria(lowest);
        CHECK(match.lock()->pos == neiRight.lock()->pos);
    }
    SUBCASE("By highest cost")
    {
        auto highest = [](std::weak_ptr<Node> a, std::weak_ptr<Node> b) {
            return a.lock()->cost > b.lock()->cost;
        };
        auto match = sOrigin->GetNeighbourBySortCriteria(highest);
        CHECK(match.lock()->pos == neiUp.lock()->pos);
    }
    SUBCASE("No neighbours")
    {
        auto otherNode = mg.CreateNode({1, 1}, 0);
        auto whatever = [](std::weak_ptr<Node> a, std::weak_ptr<Node> b) {
            return true;
        };

        auto noMatch = otherNode.lock()->GetNeighbourBySortCriteria(whatever);
        CHECK(noMatch.lock() == nullptr);
    }
}
