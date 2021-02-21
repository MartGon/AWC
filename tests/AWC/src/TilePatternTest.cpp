#include "doctest.h"

#include <Utils.h>
#include <Utils.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/TilePattern/TilePatternDescComp.h>
#include <AWC/TilePattern/TilePatternDescDecorator.h>
#include <AWC/TilePattern/TilePattern.h>
#include <AWC/TilePattern/TilePatternComp.h>
#include <AWC/TilePattern/TilePatternConstraints.h>

#include <Tests/AWC/TilePatternTest.h>

#include <iostream>

std::vector<Vector2> TilePatternTest::GetUnreachableTiles(const Map& map, const Directions& reachableTiles)
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

TEST_CASE("TilePattern pathfinding test")
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

    // TilePatternDesc
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, w, n, s};
    auto manhattanDesc = TilePatternDesc::Create(directions);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(seaTileType.GetId(), 4);
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // TilePatternConstraints
    TilePatternConstraints tpc{tileCostTable, unitCostTable, 0, 4};

    SUBCASE("Check CalculateTilePattern")
    {
        auto tp = manhattanDesc->CalculateTilePattern(map, {0, 0}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}, {0, 2}};
        auto unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);

        CHECK(tp->IsTileInPattern({0, 0}) == true);
        CHECK(tp->IsTileInPattern({1, 0}) == true);
        CHECK(tp->IsTileInPattern({1, 1}) == true);
        CHECK(tp->IsTileInPattern({1, 2}) == true);
        CHECK(tp->IsTileInPattern({0, 2}) == true);

        CHECK(tp->IsTileInPattern({0, 3}) == false);

        CHECK(tp->GetTileCost({0, 0}) == 0);
        CHECK(tp->GetTileCost({1, 0}) == 1);
        CHECK(tp->GetTileCost({1, 1}) == 2);
        CHECK(tp->GetTileCost({1, 2}) == 3);
        CHECK(tp->GetTileCost({0, 2}) == 4);

        CHECK(tp->GetPathToTile({0, 2}) == path);

        CHECK(tp->GetOrigin() == Vector2{0, 0});

        // Tiles in pattern range
        auto tilePatternReachableTiles = tp->GetTilesPosInPattern();
        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
    SUBCASE("Check CalculateTilePattern with destitnation")
    {
        auto tp = manhattanDesc->CalculateTilePattern(map, {0, 0}, Vector2{1, 2}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}};
        auto unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);
        std::vector<Vector2> path = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};

        CHECK(tp->IsTileInPattern({0, 0}) == true);
        CHECK(tp->IsTileInPattern({1, 0}) == true);
        CHECK(tp->IsTileInPattern({1, 1}) == true);
        CHECK(tp->IsTileInPattern({1, 2}) == true);

        CHECK(tp->IsTileInPattern({0, 2}) == false);
        CHECK(tp->IsTileInPattern({0, 3}) == false);

        CHECK(tp->GetTileCost({0, 0}) == 0);
        CHECK(tp->GetTileCost({1, 0}) == 1);
        CHECK(tp->GetTileCost({1, 1}) == 2);
        CHECK(tp->GetTileCost({1, 2}) == 3);

        CHECK(tp->GetPathToTile({1, 2}) == path);

        CHECK(tp->GetOrigin() == Vector2{0, 0});

        // Tiles in pattern range
        auto tilePatternReachableTiles = tp->GetTilesPosInPattern();

        CHECK(tilePatternReachableTiles.size() == tiles.size());

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}