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
#include <Tests/AWC/TilePatternCompTest.h>

#include <iostream>

TEST_CASE("TilePattern Composition Union test")
{   
    // Map 
    Map map{3, 3};
    
    TileType grassTileType{0, "Grass"};

    MapUtils::FillMap(map, grassTileType);

    // TilePatternDesc - Queen
    auto rookDesc = TilePatternCompTest::GetRookDesc();
    auto bishopDesc = TilePatternCompTest::GetBishopDesc();
    auto queenDesc = std::make_shared<TilePatternDescUnion>(rookDesc, bishopDesc);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // TilePatternConstraints
    TilePatternConstraints tpc{tileCostTable, unitCostTable, 0, 10};

    SUBCASE("Check CalculateTilePattern with composition")
    {
        TilePatternCompTest::CheckQueenDesc(queenDesc);
    }
}

TEST_CASE("TilePattern Composition Diff test")
{   
    // Map 
    Map map{5, 5};
    
    TileType grassTileType{0, "Grass"};
    TileType seaTileType{1, "Sea"};

    MapUtils::FillMap(map, grassTileType);

     // TilePatternDesc - Moore
    Vector2 e = {1, 0};
    Vector2 ne = {1, 1};
    Vector2 se = {1, -1};
    Vector2 w = {-1, 0};
    Vector2 nw = {-1, 1};
    Vector2 sw = {-1, -1};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, ne, se, w, nw, sw, n, s};
    auto mooreDesc = TilePatternDesc::Create(directions);
    auto towRangeMooreDesc = std::make_shared<TPDStaticRange>(mooreDesc, 2);

    // TilePatternDesc - One Range Diagonal
    std::vector<Vector2> dDirections = {ne, nw, se, sw};
    DirectionsTable lockedDT = {
        {ne, {ne}},
        {nw, {nw}},
        {se, {se}},
        {sw, {sw}}
    };
    auto diagonalDesc = TilePatternDesc::CreateByLocked(dDirections, lockedDT);
    auto oneDiagonalDesc = std::make_shared<TPDStaticRange>(diagonalDesc, 1, 1);

    // TilePatternDescDiff
    auto tpdd = std::make_shared<TilePatternDescDiff>(towRangeMooreDesc, oneDiagonalDesc);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // TilePatternConstraints
    TilePatternConstraints tpc{tileCostTable, unitCostTable, 0, 2};

    SUBCASE("Check CalculateTilePattern with composition")
    {
        auto tp = tpdd->CalculateTilePattern(map, {2, 2}, tpc);
        std::vector<Vector2> tiles = {
                                      {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, 
                                      {0, 3},         {2, 3},         {4, 3},   
                                      {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, 
                                      {0, 1},         {2, 1},         {4, 1},
                                      {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}
                                     };
        std::vector<Vector2> unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);

        // Moore part
        CHECK(tp->IsTileInPattern({0, 4}) == true);
        CHECK(tp->IsTileInPattern({4, 4}) == true);
        CHECK(tp->IsTileInPattern({0, 0}) == true);
        CHECK(tp->IsTileInPattern({4, 0}) == true);

        CHECK(tp->IsTileInPattern({2, 3}) == true);
        CHECK(tp->IsTileInPattern({2, 2}) == true);
        CHECK(tp->IsTileInPattern({1, 2}) == true);
        CHECK(tp->IsTileInPattern({3, 2}) == true);

        // Diagonal part
        CHECK(tp->IsTileInPattern({1, 1}) == false);
        CHECK(tp->IsTileInPattern({1, 3}) == false);
        CHECK(tp->IsTileInPattern({3, 1}) == false);
        CHECK(tp->IsTileInPattern({3, 3}) == false);

        // Cost 
        CHECK(tp->GetTileCost({0, 4}) == 2);
        CHECK(tp->GetTileCost({2, 3}) == 1);

        // Paths
        CHECK(tp->GetPathToTile({2, 3}) == std::vector<Vector2>{{2, 2}, {2, 3}});
        CHECK(tp->GetPathToTile({2, 4}) == std::vector<Vector2>{{2, 2}, {2, 3}, {2 ,4}});

        // Should walk through {1, 3} despite the fact that it cannot go to it.
        CHECK(tp->GetPathToTile({0, 4}) == std::vector<Vector2>{{2, 2}, {1, 3}, {0, 4}});

        // Origin
        CHECK(tp->GetOrigin() == Vector2{2, 2});

        // Tiles in pattern range
        auto tilePatternReachableTiles = tp->GetTilesPosInPattern();
        CHECK(tilePatternReachableTiles.size() == tiles.size());

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}

TEST_CASE("TilePattern Composition Intersect test")
{   
    // Map 
    Map map{5, 5};
    
    TileType grassTileType{0, "Grass"};

    MapUtils::FillMap(map, grassTileType);

     // TilePatternDesc - Moore
    Vector2 e = {1, 0};
    Vector2 ne = {1, 1};
    Vector2 se = {1, -1};
    Vector2 w = {-1, 0};
    Vector2 nw = {-1, 1};
    Vector2 sw = {-1, -1};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, ne, se, w, nw, sw, n, s};
    auto mooreDesc = TilePatternDesc::Create(directions);
    auto oneRangeMoore = std::make_shared<TPDStaticRange>(mooreDesc, 1);

    // TilePatternDesc - Manhattan
    std::vector<Vector2> dDirections = {e, w, s, n};
    auto manhattan = TilePatternDesc::Create(dDirections);
    auto oneRangeManhattan = std::make_shared<TPDStaticRange>(manhattan, 1);

    // TilePatternDescDiff
    auto tpdd = std::make_shared<TilePatternDescIntersect>(oneRangeMoore, oneRangeManhattan);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // TilePatternConstraints
    TilePatternConstraints tpc{tileCostTable, unitCostTable, 0, 2};

    SUBCASE("Check CalculateTilePattern with composition")
    {
        auto tp = tpdd->CalculateTilePattern(map, {2, 2}, tpc);
        std::vector<Vector2> tiles = {
                                                {2, 3},         
                                        {1, 2}, {2, 2}, {3, 2},
                                                {2, 1},              
                                     };
        std::vector<Vector2> unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);

        // Manhattan
        CHECK(tp->IsTileInPattern({2, 3}) == true);
        CHECK(tp->IsTileInPattern({1, 2}) == true);
        CHECK(tp->IsTileInPattern({2, 2}) == true);
        CHECK(tp->IsTileInPattern({3, 2}) == true);
        CHECK(tp->IsTileInPattern({2, 1}) == true);

        // Diagonal is unreachable
        CHECK(tp->IsTileInPattern({1, 3}) == false);
        CHECK(tp->IsTileInPattern({3, 3}) == false);
        CHECK(tp->IsTileInPattern({1, 1}) == false);
        CHECK(tp->IsTileInPattern({3, 1}) == false);

        // Cost 
        CHECK(tp->GetTileCost({2, 3}) == 1);
        CHECK(tp->GetTileCost({2, 2}) == 0);

        // Paths
        CHECK(tp->GetPathToTile({2, 3}) == std::vector<Vector2>{{2, 2}, {2, 3}});
        CHECK(tp->GetPathToTile({3, 2}) == std::vector<Vector2>{{2, 2}, {3, 2}});

        // Origin
        CHECK(tp->GetOrigin() == Vector2{2, 2});

        // Tiles in pattern range
        auto tilePatternReachableTiles = tp->GetTilesPosInPattern();
        CHECK(tilePatternReachableTiles.size() == tiles.size());

        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}

TilePatternDescIPtr TilePatternCompTest::GetRookDesc()
{
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directionsR = {e, w, n, s};
    DirectionsTable lockedDirTableR = {
        {e, {e}},
        {w, {w}},
        {n, {n}},
        {s, {s}}
    };
    auto rookDesc = TilePatternDesc::CreateByLocked(directionsR, lockedDirTableR);
    return rookDesc;
}

TilePatternDescIPtr TilePatternCompTest::GetBishopDesc()
{
    // TilePatternDesc - Bishop
    Vector2 ne = {1, 1};
    Vector2 nw = {-1, 1};
    Vector2 se = {1, -1};
    Vector2 sw = {-1, -1};
    std::vector<Vector2> directionsB = {ne, nw, se, sw};
    DirectionsTable lockedDirTableB = {
        {ne, {ne}},
        {nw, {nw}},
        {se, {se}},
        {sw, {sw}}
    };
    auto bishopDesc = TilePatternDesc::CreateByLocked(directionsB, lockedDirTableB);
    return bishopDesc;
}

void TilePatternCompTest::CheckQueenDesc(TilePatternDescIPtr queenDesc)
{
     // Map 
    Map map{3, 3};
    TileType grassTileType{0, "Grass"};
    MapUtils::FillMap(map, grassTileType);

    // CostTable
    std::shared_ptr<CostTable> tileCostTable{new CostTable};
    tileCostTable->SetCost(grassTileType.GetId(), 1);

    std::shared_ptr<CostTable> unitCostTable{new CostTable};

    // TilePatternConstraints
    TilePatternConstraints tpc{tileCostTable, unitCostTable, 0, 10};

    auto tp = queenDesc->CalculateTilePattern(map, {0, 0}, tpc);
    std::vector<Vector2> tiles = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {2, 0}, {2, 2}};
    std::vector<Vector2> unreachableTiles = TilePatternTest::GetUnreachableTiles(map, tiles);

    // Rook part
    CHECK(tp->IsTileInPattern({0, 0}) == true);
    CHECK(tp->IsTileInPattern({1, 0}) == true);
    CHECK(tp->IsTileInPattern({0, 1}) == true);
    CHECK(tp->IsTileInPattern({2, 0}) == true);
    CHECK(tp->IsTileInPattern({0, 2}) == true);

    // Bishop part
    CHECK(tp->IsTileInPattern({1, 1}) == true);
    CHECK(tp->IsTileInPattern({2, 2}) == true);

    // Not reachable tiles
    CHECK(tp->IsTileInPattern({1, 2}) == false);
    CHECK(tp->IsTileInPattern({2, 1}) == false);

    // Cost 
    CHECK(tp->GetTileCost({1, 1}) == 1);
    CHECK(tp->GetTileCost({2, 2}) == 2);

    // Paths - Rook
    CHECK(tp->GetPathToTile({0, 1}) == std::vector<Vector2>{{0, 0}, {0, 1}});
    CHECK(tp->GetPathToTile({0, 2}) == std::vector<Vector2>{{0, 0}, {0, 1}, {0, 2}});

    // Paths - Bishop
    CHECK(tp->GetPathToTile({1, 1}) == std::vector<Vector2>{{0, 0}, {1, 1}});
    CHECK(tp->GetPathToTile({2, 2}) == std::vector<Vector2>{{0, 0}, {1, 1}, {2, 2}});

    // Origin
    CHECK(tp->GetOrigin() == Vector2{0, 0});

    // Tiles in pattern range
    auto tilePatternReachableTiles = tp->GetTilesPosInPattern();
    CHECK(tilePatternReachableTiles.size() == tiles.size());

    for(auto tile : tiles)
        CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
    for(auto tile : unreachableTiles)
        CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
}