#include "doctest.h"

#include <Utils/Vector2.h>
#include <Utils/STLUtils.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePatternDescriptorComp.h>
#include <AWC/TilePatternDescriptorDecorator.h>
#include <AWC/TilePattern.h>
#include <AWC/TilePatternComp.h>
#include <AWC/TilePatternConstraints.h>

#include <TilePatternTest.h>

#include <iostream>

TEST_CASE("TilePattern Decorator test")
{   
    // Map 
    Map map{5, 5};
    
    TileType grassTileType{0, "Grass"};
    TileType seaTileType{1, "Sea"};

    MapUtils::FillMap(map, grassTileType);
    
    // Put some sea tiles
    std::vector<Vector2> diagonalPos = {{0, 0}, {2, 2}, {0, 2}, {2, 0}};
    for(auto pos : diagonalPos)
        map.SetTile(pos, seaTileType.CreateTile());

    // TilePatternDescriptor - Moore
    Vector2 e = {1, 0};
    Vector2 ne = {1, 1};
    Vector2 se = {1, -1};
    Vector2 w = {-1, 0};
    Vector2 nw = {-1, 1};
    Vector2 sw = {-1, -1};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, ne, se, w, nw, sw, n, s};
    auto mooreDescriptor = TilePatternDescriptor::Create(directions);
    
    // Fixed CostTable
    FixedCostTable fixedTileCostTable{1};

    // Fixed Range TPD
    CostTable unitCostTable;
    auto mooreDescriptorFixedRange = std::make_shared<TPDFixedRange>(mooreDescriptor, 1, 0);

    // Normal TPC
    TilePatternConstraints constraints{fixedTileCostTable, unitCostTable, 8, 0};

    SUBCASE("Check CalculateTilePattern with composition")
    {
        auto tp = mooreDescriptorFixedRange->CalculateTilePattern({1, 1}, map, constraints);
        std::vector<Vector2> tiles = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {2, 0}, {2, 2}, {1, 2}, {2, 1}};
        std::vector<Vector2> unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);

        // Moore reachable pos from {1, 1}
        CHECK(tp->IsTileInPattern({0, 0}) == true);
        CHECK(tp->IsTileInPattern({1, 0}) == true);
        CHECK(tp->IsTileInPattern({0, 1}) == true);
        CHECK(tp->IsTileInPattern({2, 0}) == true);
        CHECK(tp->IsTileInPattern({0, 2}) == true);
        CHECK(tp->IsTileInPattern({1, 1}) == true);
        CHECK(tp->IsTileInPattern({2, 2}) == true);

        // Not reachable tiles
        CHECK(tp->IsTileInPattern({3, 3}) == false);
        CHECK(tp->IsTileInPattern({0, 3}) == false);

        // Cost 
        CHECK(tp->GetTileCost({0, 0}) == 1);
        CHECK(tp->GetTileCost({2, 2}) == 1);

        // Paths - Rook
        CHECK(tp->GetPathToTile({0, 1}) == std::vector<Vector2>{{1, 1}, {0, 1}});
        CHECK(tp->GetPathToTile({0, 2}) == std::vector<Vector2>{{1, 1}, {0, 2}});

        // Origin
        CHECK(tp->GetOrigin() == Vector2{1, 1});

        // Tiles in pattern range
        auto tilePatternReachableTiles = tp->GetTilesPosInPattern();
        CHECK(tilePatternReachableTiles.size() == 9);

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}
