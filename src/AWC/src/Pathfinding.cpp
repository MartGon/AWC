#include <AWC/Pathfinding.h>

#include <AWC/Map.h>
#include <AWC/Area/Graph.h>
#include <AWC/Area/Area.h>
#include <AWC/Area/AreaConstraints.h>
#include <AWC/Area/AreaDesc.h>

#include <Utils.h>

#include <queue>

using namespace Pathfinding;

Graph Pathfinding::Dijkstra(Vector2 origin, Params params)
{
    Graph tg;
    auto originNode = tg.CreateNode(origin, 0);

    // Create prioQueue
    // PrioQueue pop values in reverse order
    auto greater = [](NodePtr a, NodePtr b) { return a.lock()->cost > b.lock()->cost;};
    std::priority_queue<NodePtr, std::vector<NodePtr>, decltype(greater)> prioQueue{greater};
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
   
            auto constraints = params.constraints;
            uint neiCost = GetTileCost(map, constraints, sharedNei->pos);
            
            // Ignore tiles that are at max cost
            // Avoids unnecessary work when maxRange = max;
            const auto maxDist = std::numeric_limits<unsigned int>::max();
            if(neiCost < maxDist)
            {
                // Check if accumulated cost to this neighbour is lower than previous
                uint accumulatedCost = PrimitiveUtils::NoOverflowSum(node->cost, neiCost);
                const bool isInRange = accumulatedCost <= constraints.range.maxRange;
                if(accumulatedCost < sharedNei->cost && isInRange)
                {
                    // Push it to queue if that's the case
                    sharedNei->cost = accumulatedCost;
                    prioQueue.push(nei);
                }
            }
        }
    }
    
    return tg;
}

Directions Pathfinding::GetDiscoverDirections(NodePtr tileNode, Params params)
{
    auto directions = params.tpd.GetOriginDirections();

    auto movement = GetMovementToOrigin(tileNode);
    directions = params.tpd.GetLockedDirections(movement);
    directions = GetValidDirections(tileNode, directions, params.map);

    return directions;
}

Vector2 Pathfinding::GetMovementToOrigin(NodePtr tileNode)
{
    Vector2 movement{0, 0};

    auto sNode = tileNode.lock();
    auto lowestCost = [](NodePtr a, NodePtr b) {
            return a.lock()->cost < b.lock()->cost;
        };

    auto neighbour = sNode->GetNeighbourBySortCriteria(lowestCost);
    if(auto sNeigbour = neighbour.lock())
        movement = sNode->pos - sNeigbour->pos;
    
    return movement;
}

Directions Pathfinding::GetValidDirections(NodePtr tileNode, Directions directions, const Map& map)
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

unsigned int Pathfinding::GetTileCost(const Map& map, const AreaConstraints& tpc, Vector2 pos)
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