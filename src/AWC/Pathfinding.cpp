#include "AWC/Pathfinding.h"

#include <AWC/TilePattern/TileGraph.h>
#include <AWC/TilePattern/TilePattern.h>
#include <AWC/TilePattern/TilePatternConstraints.h>
#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/Map.h>
#include <Utils/STLUtils.h>

#include <queue>

using namespace Pathfinding;

TileGraph Pathfinding::Dijkstra(Vector2 origin, Params params)
{
    TileGraph tg;
    auto originNode = tg.CreateNode(origin, 0);

    // Create prioQueue
    // PrioQueue pop values in reverse order
    auto greater = [](TileNodePtr a, TileNodePtr b) { return a.lock()->cost > b.lock()->cost;};
    std::priority_queue<TileNodePtr, std::vector<TileNodePtr>, decltype(greater)> prioQueue{greater};
    prioQueue.push(originNode);

    while(!prioQueue.empty())
    {
        // Pop first member
        auto node = prioQueue.top().lock();
        prioQueue.pop();

        auto destination = params.dest;

        if(destination.has_value() && destination.value() == node->pos)
            break;

        // Get its neighbours
        auto map = params.map;
        auto discoverDirections = GetDiscoverDirections(node, params);
        auto neighbours = tg.DiscoverNeighbours(node->pos, discoverDirections);
        for(const auto& nei : neighbours)
        {
            auto sharedNei = nei.lock();

            // Check if accumulated cost to this neighbour is lower than previous
            auto constraints = params.constraints;
            uint neiCost = GetTileCost(map, constraints, sharedNei->pos);
            uint calculatedCost = PrimitiveUtils::NoOverflowSum(node->cost, neiCost);
            if(calculatedCost < sharedNei->cost && calculatedCost <= constraints.range.maxRange)
            {
                // Push it to queue if that's the case
                sharedNei->cost = calculatedCost;
                prioQueue.push(nei);
            }
        }
    }
    
    return tg;
}

Directions Pathfinding::GetDiscoverDirections(TileNodePtr tileNode, Params params)
{
    auto directions = params.tpd.GetOriginDirections();

    auto movement = GetMovementToOrigin(tileNode);
    directions = params.tpd.GetLockedDirections(movement);
    directions = GetValidDirections(tileNode, directions, params.map);

    return directions;
}

Vector2 Pathfinding::GetMovementToOrigin(TileNodePtr tileNode)
{
    Vector2 movement{0, 0};

    auto sTileNode = tileNode.lock();
    auto lowestCost = [](TileNodePtr a, TileNodePtr b) {
            return a.lock()->cost < b.lock()->cost;
        };

    auto neighbour = sTileNode->GetNeighbourBySortCriteria(lowestCost);
    if(auto sNeigbour = neighbour.lock())
        movement = sTileNode->pos - sNeigbour->pos;
    
    return movement;
}

Directions Pathfinding::GetValidDirections(TileNodePtr tileNode, Directions directions, const Map& map)
{
    auto nodePos = tileNode.lock()->pos;

    std::vector<Vector2> validDirections;
    for(auto dir : directions)
    {
        auto neigbourPos = nodePos + dir;
        if(map.IsPositionValid(neigbourPos))
            validDirections.push_back(dir);
    }

    return validDirections;
}

unsigned int Pathfinding::GetTileCost(const Map& map, const TilePatternConstraints& tpc, Vector2 pos)
{
    auto tile = map.GetTile(pos);
    auto cost = tpc.GetTileCost(tile->GetId());
  
    if(auto unit = map.GetUnit(pos))
    {   
        auto extraCost = tpc.GetUnitCost(unit->GetTypeId());
        cost = PrimitiveUtils::NoOverflowSum(cost, extraCost);
    }

    return cost;
}