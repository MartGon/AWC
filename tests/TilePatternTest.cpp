#include "doctest.h"

#include <Utils/Vector2.h>
#include <Utils/STLUtils.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePatternDescriptorComp.h>
#include <AWC/TilePattern.h>
#include <AWC/TilePatternConstraints.h>

std::vector<Vector2> GetUnreachableTiles(const Map& map, const Directions& reachableTiles)
{
    std::vector<Vector2> unreachableTiles;

    for(int x = 0; x < map.GetWidht(); x++)
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

    // TilePatternDescriptor
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    std::vector<Vector2> directions = {e, w, n, s};
    auto manhattanDescriptor = TilePatternDescriptor::Create(directions);

    // CostTable
    CostTable tileCostTable;
    tileCostTable.SetCost(seaTileType.GetId(), 4);
    tileCostTable.SetCost(grassTileType.GetId(), 1);

    CostTable unitCostTable;

    // TilePatternConstraints
    TilePatternConstraints tpc{map, tileCostTable, unitCostTable, 4};

    SUBCASE("Check CalculateTilePattern")
    {
        auto tp = manhattanDescriptor->CalculateTilePattern({0, 0}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}, {0, 2}};
        auto unreachableTiles = GetUnreachableTiles(map, tiles);

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
        auto tp = manhattanDescriptor->CalculateTilePattern({0, 0}, Vector2{1, 2}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}};
        auto unreachableTiles = GetUnreachableTiles(map, tiles);
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
        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}

TEST_CASE("TilePattern Composition test")
{   
    // Map 
    Map map{3, 3};
    
    TileType grassTileType{0, "Grass"};

    MapUtils::FillMap(map, grassTileType);

    // TilePatternDescriptor - Rook
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
    auto rookDescriptor = TilePatternDescriptor::CreateByLocked(directionsR, lockedDirTableR);

    // TilePatternDescriptor - Bishop
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
    auto bishopDescriptor = TilePatternDescriptor::CreateByLocked(directionsB, lockedDirTableB);

    // TilePatternDescriptor - Queen
    auto queenDescriptor = std::make_shared<TilePatternDescriptorUnion>(rookDescriptor, bishopDescriptor);

    // CostTable
    CostTable tileCostTable;
    tileCostTable.SetCost(grassTileType.GetId(), 1);

    CostTable unitCostTable;

    // TilePatternConstraints
    TilePatternConstraints tpc{map, tileCostTable, unitCostTable, 10};

    SUBCASE("Check CalculateTilePattern with composition")
    {
        auto tp = queenDescriptor->CalculateTilePattern({0, 0}, tpc);
        std::vector<Vector2> tiles = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {2, 0}, {2, 2}};
        std::vector<Vector2> unreachableTiles = GetUnreachableTiles(map, tiles);

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
        for(auto tile : tiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == true);
        for(auto tile : unreachableTiles)
            CHECK(VectorUtils::IsInside(tilePatternReachableTiles, tile) == false);
    }
}