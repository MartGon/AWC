#include "doctest.h"

#include <Utils.h>
#include <Utils.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/Area/AreaDesc.h>
#include <AWC/Area/AreaDescComp.h>
#include <AWC/Area/AreaDescDecorator.h>
#include <AWC/Area/Area.h>
#include <AWC/Area/AreaComp.h>
#include <AWC/Area/AreaConstraints.h>

#include <Tests/AWC/AreaTest.h>

#include <iostream>

std::vector<Vector2> AreaTest::GetUnreachableTiles(const Map& map, const Directions& reachableTiles)
{
    std::vector<Vector2> unreachableTiles;

    for(int x = 0; x < map.GetWidth(); x++)
    {
        for(int y = 0; y < map.GetHeight(); y++)
        {
            Vector2 pos{x, y};
            if(!VectorUtils::IsInside(reachableTiles, pos))
                unreachableTiles.push_back(pos);
        }
    }

    return unreachableTiles;
}

TEST_CASE("Area pathfinding test")
{   
    // Map 
    Map map{10, 10};
    
    TileType grassTileType{0, "Grass"};
    TileType seaTileType{1, "Sea"};

    MapUtils::FillMap(map, seaTileType);
    
    // Set a path of grass
    std::vector<Vector2> path = {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {0, 2}};
    for(auto pos : path)
        map.SetTile(pos.x, pos.y, grassTileType.CreateTile());

    // AreaDesc
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, w, n, s};
    auto manhattanDesc = AreaDesc::Create(directions);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(seaTileType.GetId(), 4);
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // AreaConstraints
    AreaConstraints tpc{tileCostTable, unitCostTable, 0, 4};

    SUBCASE("Check CalculateArea")
    {
        auto tp = manhattanDesc->CalculateArea(map, {0, 0}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}, {0, 2}};
        auto unreachableTiles = AreaTest::GetUnreachableTiles(map, tiles);

        CHECK(tp->ContainsTile({0, 0}) == true);
        CHECK(tp->ContainsTile({1, 0}) == true);
        CHECK(tp->ContainsTile({1, 1}) == true);
        CHECK(tp->ContainsTile({1, 2}) == true);
        CHECK(tp->ContainsTile({0, 2}) == true);

        CHECK(tp->ContainsTile({0, 3}) == false);

        CHECK(tp->GetTileCost({0, 0}) == 0);
        CHECK(tp->GetTileCost({1, 0}) == 1);
        CHECK(tp->GetTileCost({1, 1}) == 2);
        CHECK(tp->GetTileCost({1, 2}) == 3);
        CHECK(tp->GetTileCost({0, 2}) == 4);

        CHECK(tp->GetPathToTile({0, 2}) == path);

        CHECK(tp->GetOrigin() == Vector2{0, 0});

        // Tiles in pattern range
        auto areaReachableTiles = tp->GetContainedTiles();
        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == false);
    }
    SUBCASE("Check CalculateArea with destitnation")
    {
        auto tp = manhattanDesc->CalculateArea(map, {0, 0}, Vector2{1, 2}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}};
        auto unreachableTiles = AreaTest::GetUnreachableTiles(map, tiles);
        std::vector<Vector2> path = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};

        CHECK(tp->ContainsTile({0, 0}) == true);
        CHECK(tp->ContainsTile({1, 0}) == true);
        CHECK(tp->ContainsTile({1, 1}) == true);
        CHECK(tp->ContainsTile({1, 2}) == true);

        CHECK(tp->ContainsTile({0, 2}) == false);
        CHECK(tp->ContainsTile({0, 3}) == false);

        CHECK(tp->GetTileCost({0, 0}) == 0);
        CHECK(tp->GetTileCost({1, 0}) == 1);
        CHECK(tp->GetTileCost({1, 1}) == 2);
        CHECK(tp->GetTileCost({1, 2}) == 3);

        CHECK(tp->GetPathToTile({1, 2}) == path);

        CHECK(tp->GetOrigin() == Vector2{0, 0});

        // Tiles in pattern range
        auto areaReachableTiles = tp->GetContainedTiles();

        CHECK(areaReachableTiles.size() == tiles.size());

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == false);
    }
}