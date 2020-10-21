#include "doctest.h"

#include <Utils/Vector2.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePattern.h>
#include <AWC/TilePatternConstraints.h>

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
    auto manhattanDescriptor = TilePatternDescriptor::CreateTilePatternDescriptor(directions);

    // CostTable
    CostTable tileCostTable;
    tileCostTable.SetCost(seaTileType.GetId(), 4);
    tileCostTable.SetCost(grassTileType.GetId(), 1);

    CostTable unitCostTable;

    // TilePatternConstraints
    TilePatternConstraints tpc{map, tileCostTable, unitCostTable, 4};

    SUBCASE("Check if destination is in tp")
    {
        auto tp = manhattanDescriptor.CalculateTilePattern({0, 0}, tpc);

        CHECK(tp->IsTileInPattern({1, 1}) == true);
        CHECK(tp->IsTileInPattern({0, 2}) == true);
        CHECK(tp->GetTileCost({0, 2}) == 4);
        CHECK(tp->IsTileInPattern({0, 3}) == false);
        CHECK(tp->GetPathToTile({0, 2}) == path);
    }
    SUBCASE("Check if destination is in tp with Destination")
    {
        auto tp = manhattanDescriptor.CalculateTilePattern({0, 0}, Vector2{0, 2}, tpc);

        CHECK(tp->IsTileInPattern({1, 1}) == true);
        CHECK(tp->IsTileInPattern({0, 2}) == true);
        CHECK(tp->GetTileCost({0, 2}) == 4);
        CHECK(tp->IsTileInPattern({0, 3}) == false);
        CHECK(tp->GetPathToTile({0, 2}) == path);
    }
}