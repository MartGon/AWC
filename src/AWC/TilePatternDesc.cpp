#include <AWC/TilePattern.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/TilePatternConstraints.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileGraph.h>
#include <AWC/AWCException.h>

#include <Utils/STLUtils.h>

#include <limits>
#include <queue>
#include <functional>
#include <optional>

// Public

// Factory methods
TilePatternDescPtr TilePatternDesc::CreateByLocked(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable)
{
    return std::shared_ptr<TilePatternDesc>{
        new TilePatternDesc{directions, PrepareLockedDirectionsTable(directions, lockedDirectionsTable)
        }};
}

TilePatternDescPtr  TilePatternDesc::CreateByExclusive(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    return std::shared_ptr<TilePatternDesc>{
        new TilePatternDesc{directions, GenerateLockedDirectionsTable(directions, exclusiveDirectionsTable)
        }};
}

// Constructors
TilePatternDesc::TilePatternDesc(const Directions& directions) : 
    originDirections_{directions}, lockedDirectionsTable_{GenerateDefaultLockedDirectionsTable(directions)}
{

}

TilePatternDesc::TilePatternDesc(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable) : 
    originDirections_{directions}, lockedDirectionsTable_{lockedDirectionsTable}
{

}

// Methods

Directions TilePatternDesc::GetOriginDirections()
{
    return originDirections_;
}

bool TilePatternDesc::IsOriginDirection(Vector2 dir)
{
    return VectorUtils::IsInside(originDirections_, dir);
}

void TilePatternDesc::AddOriginDirection(Vector2 dir)
{
    if(!IsOriginDirection(dir))
        originDirections_.push_back(dir);
    else
        throw AWCAlreadyExistingIndexException("TilePatternDesc::AddOriginDirection", dir);
}

void TilePatternDesc::RemoveOriginDirection(Vector2 dir)
{
    if(IsOriginDirection(dir))
    {
        VectorUtils::RemoveByValue(originDirections_, dir);
        lockedDirectionsTable_.erase(dir);
    }
    else
        throw AWCNoExistingIndexException("TilePatternDesc::RemoveOriginDirection", dir);

}

Directions TilePatternDesc::GetLockedDirections(Vector2 dir)
{
    return lockedDirectionsTable_.at(dir);
}

void TilePatternDesc::SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections)
{
    lockedDirectionsTable_[dir] = GenerateLockedDirections(originDirections_, exclusiveDirections);
}

void TilePatternDesc::SetLockedDirections(Vector2 dir, const Directions& lockedDirections)
{
    lockedDirectionsTable_[dir] = lockedDirections;
}

// Private

const Vector2 TilePatternDesc::NULL_MOVEMENT = Vector2{0, 0};

DirectionsTable TilePatternDesc::GenerateDefaultLockedDirectionsTable(const Directions& directions)
{
    DirectionsTable lockedDirectionsTable;
    lockedDirectionsTable.insert({NULL_MOVEMENT, directions});

    for(const auto dir : directions)
    {
        // The intention is to remove loops
        Vector2 neg = -dir;
        Directions lockedDirections = GenerateLockedDirections(directions, {neg});
        lockedDirectionsTable.insert({dir, lockedDirections});
    }

    return lockedDirectionsTable;
}

DirectionsTable TilePatternDesc::PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections)
{
    auto finalLockedDirectionsTable = lockedDirections;
    finalLockedDirectionsTable.insert({NULL_MOVEMENT, directions});
    return finalLockedDirectionsTable;
}

DirectionsTable TilePatternDesc::GenerateLockedDirectionsTable(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    DirectionsTable lockedDirectionsTable;
    lockedDirectionsTable.insert({NULL_MOVEMENT, directions});

    for(const auto pair : exclusiveDirectionsTable)
    {
        auto dir = pair.first;
        auto exclusiveDirections = pair.second;

        auto lockedDirectionsForDir = GenerateLockedDirections(directions, exclusiveDirections);
        lockedDirectionsTable.insert({dir, lockedDirectionsForDir});
    }

    return lockedDirectionsTable;
}

Directions TilePatternDesc::GenerateLockedDirections(const Directions& directions, 
    const Directions& exclusiveDirections)
{
    Directions lockedDirections = directions;
    for(const auto exclusiveDir : exclusiveDirections)
        VectorUtils::RemoveByValue(lockedDirections, exclusiveDir);

    return lockedDirections;
}

TilePatternIPtr TilePatternDesc::DoCalculateTilePattern(Vector2 origin, 
    std::optional<Vector2> destination, const Map& map, const TilePatternConstraints& constraints)
{
        // Create mapGraph
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

        if(destination.has_value() && destination.value() == node->pos)
            break;

        // Get its neighbours
        auto discoverDirections = GetDiscoverDirections(node, map);
        auto neighbours = tg.DiscoverNeighbours(node->pos, discoverDirections);
        for(const auto& nei : neighbours)
        {
            auto sharedNei = nei.lock();

            // Check if accumulated cost to this neighbour is lower than previous
            int neiCost = GetTileCost(map, constraints, sharedNei->pos);
            int calculatedCost = node->cost + neiCost;
            if(calculatedCost < sharedNei->cost && calculatedCost <= constraints.maxRange)
            {
                // Push it to queue if that's the case
                sharedNei->cost = calculatedCost;
                prioQueue.push(nei);
            }
        }
    }

    // Create TilePattern
    auto tp = TilePatternPtr(new TilePattern{origin, tg, constraints.maxRange, constraints.minRange});

    return tp;
}

Directions TilePatternDesc::GetDiscoverDirections(TileNodePtr tileNode, const Map& map)
{
    auto directions = GetOriginDirections();

    auto movement = GetMovementToOrigin(tileNode);
    directions = GetLockedDirections(movement);
    directions = GetValidDirections(tileNode, directions, map);

    return directions;
}

Vector2 TilePatternDesc::GetMovementToOrigin(TileNodePtr tileNode)
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

Directions TilePatternDesc::GetValidDirections(TileNodePtr tileNode, Directions directions, const Map& map)
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

unsigned int TilePatternDesc::GetTileCost(const Map& map, const TilePatternConstraints& tpc, Vector2 pos)
{
    auto tile = map.GetTile(pos);
    auto cost = tpc.GetTileCost(tile->GetId());

    return cost;
}