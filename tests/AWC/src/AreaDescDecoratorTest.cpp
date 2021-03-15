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

TEST_CASE("Area Fixed Range Decorator test")
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

    // AreaDesc - Moore
    Vector2 e = {1, 0};
    Vector2 ne = {1, 1};
    Vector2 se = {1, -1};
    Vector2 w = {-1, 0};
    Vector2 nw = {-1, 1};
    Vector2 sw = {-1, -1};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, ne, se, w, nw, sw, n, s};
    auto mooreDesc = AreaDesc::Create(directions);
    
    // Fixed CostTable
    CostTableIPtr tileCostTable{new FixedCostTable{1}};

    // Fixed Range TPD
    CostTableIPtr unitCostTable{new CostTable};
    auto mooreDescFixedRange = std::make_shared<TPDStaticRange>(mooreDesc, 1, 0);

    // Normal TPC
    AreaConstraints constraints{tileCostTable, unitCostTable, 8, 0};

    SUBCASE("Check CalculateArea with composition")
    {
        auto tp = mooreDescFixedRange->CalculateArea(map, {1, 1}, constraints);
        std::vector<Vector2> tiles = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {2, 0}, {2, 2}, {1, 2}, {2, 1}};
        std::vector<Vector2> unreachableTiles = AreaTest::GetUnreachableTiles(map, tiles);

        // Moore reachable pos from {1, 1}
        CHECK(tp->ContainsTile({0, 0}) == true);
        CHECK(tp->ContainsTile({1, 0}) == true);
        CHECK(tp->ContainsTile({0, 1}) == true);
        CHECK(tp->ContainsTile({2, 0}) == true);
        CHECK(tp->ContainsTile({0, 2}) == true);
        CHECK(tp->ContainsTile({1, 1}) == true);
        CHECK(tp->ContainsTile({2, 2}) == true);

        // Not reachable tiles
        CHECK(tp->ContainsTile({3, 3}) == false);
        CHECK(tp->ContainsTile({0, 3}) == false);

        // Cost 
        CHECK(tp->GetTileCost({0, 0}) == 1);
        CHECK(tp->GetTileCost({2, 2}) == 1);

        // Paths - Rook
        CHECK(tp->GetPathToTile({0, 1}) == std::vector<Vector2>{{1, 1}, {0, 1}});
        CHECK(tp->GetPathToTile({0, 2}) == std::vector<Vector2>{{1, 1}, {0, 2}});

        // Origin
        CHECK(tp->GetOrigin() == Vector2{1, 1});

        // Tiles in pattern range
        auto areaReachableTiles = tp->GetContainedTiles();
        CHECK(areaReachableTiles.size() == 9);

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == false);
    }
}

TEST_CASE("Area Fixed Cost Decorator test")
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

    // AreaDesc - Moore
    Vector2 e = {1, 0};
    Vector2 ne = {1, 1};
    Vector2 se = {1, -1};
    Vector2 w = {-1, 0};
    Vector2 nw = {-1, 1};
    Vector2 sw = {-1, -1};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, ne, se, w, nw, sw, n, s};
    auto mooreDesc = AreaDesc::Create(directions);
    
    // Fixed CostTable
    auto fixedTileCostTable = std::make_shared<FixedCostTable>(1);
    auto unitTileCostTable = std::make_shared<FixedCostTable>(0);

    // Fixed Range TPD
    CostTableIPtr unitCostTable{new CostTable};
    auto mooreDescFixedRange = std::make_shared<TPDStaticRange>(mooreDesc, 1, 0);
    auto mooreDescFixedCost = std::make_shared<TPDStaticCost>(mooreDescFixedRange, fixedTileCostTable, unitTileCostTable);

    // Normal TPC
    std::shared_ptr<CostTable> normalCostTable{new CostTable};
    normalCostTable->SetCost(0, 1);
    normalCostTable->SetCost(1, 4);
    AreaConstraints constraints{normalCostTable, unitCostTable, 8, 0};

    SUBCASE("Check CalculateArea with composition")
    {
        auto tp = mooreDescFixedCost->CalculateArea(map, {1, 1}, constraints);
        std::vector<Vector2> tiles = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {2, 0}, {2, 2}, {1, 2}, {2, 1}};
        std::vector<Vector2> unreachableTiles = AreaTest::GetUnreachableTiles(map, tiles);

        // Moore reachable pos from {1, 1}
        CHECK(tp->ContainsTile({0, 0}) == true);
        CHECK(tp->ContainsTile({1, 0}) == true);
        CHECK(tp->ContainsTile({0, 1}) == true);
        CHECK(tp->ContainsTile({2, 0}) == true);
        CHECK(tp->ContainsTile({0, 2}) == true);
        CHECK(tp->ContainsTile({1, 1}) == true);
        CHECK(tp->ContainsTile({2, 2}) == true);

        // Not reachable tiles
        CHECK(tp->ContainsTile({3, 3}) == false);
        CHECK(tp->ContainsTile({0, 3}) == false);

        // Cost 
        CHECK(tp->GetTileCost({0, 0}) == 1);
        CHECK(tp->GetTileCost({2, 2}) == 1);

        // Paths - Rook
        CHECK(tp->GetPathToTile({0, 1}) == std::vector<Vector2>{{1, 1}, {0, 1}});
        CHECK(tp->GetPathToTile({0, 2}) == std::vector<Vector2>{{1, 1}, {0, 2}});

        // Origin
        CHECK(tp->GetOrigin() == Vector2{1, 1});

        // Tiles in pattern range
        auto areaReachableTiles = tp->GetContainedTiles();
        CHECK(areaReachableTiles.size() == 9);

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(areaReachableTiles, tile) == false);
    }
}
