#include <AWC/TilePattern.h>
#include <AWC/TilePatternDescriptor.h>
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

// TilePatternDescriptorI



// Public

// Factory methods
TilePatternDescriptorPtr TilePatternDescriptor::CreateByLocked(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable)
{
    return std::shared_ptr<TilePatternDescriptor>{new TilePatternDescriptor{directions, lockedDirectionsTable}};
}

TilePatternDescriptorPtr  TilePatternDescriptor::CreateByExclusive(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    return std::shared_ptr<TilePatternDescriptor>{new TilePatternDescriptor{directions, GenerateLockedDirectionsTable(directions, exclusiveDirectionsTable)}};
}

// Constructors
TilePatternDescriptor::TilePatternDescriptor(const Directions& directions) : 
    directions_{directions}, lockedDirectionsTable_{GenerateDefaultLockedDirectionsTable(directions)}
{

}

TilePatternDescriptor::TilePatternDescriptor(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable) : 
    directions_{directions}, lockedDirectionsTable_{lockedDirectionsTable}
{

}

// Methods

Directions TilePatternDescriptor::GetDirections()
{
    return directions_;
}

bool TilePatternDescriptor::IsDirection(Vector2 dir)
{
    return VectorUtils::IsInside(directions_, dir);
}

void TilePatternDescriptor::AddDirection(Vector2 dir)
{
    if(!IsDirection(dir))
        directions_.push_back(dir);
    else
        throw AWCAlreadyExistingIndexException(dir);
}

void TilePatternDescriptor::RemoveDirection(Vector2 dir)
{
    if(IsDirection(dir))
    {
        VectorUtils::RemoveByValue(directions_, dir);
        lockedDirectionsTable_.erase(dir);
    }
    else
        throw AWCNoExistingIndexException(dir);

}

Directions TilePatternDescriptor::GetLockedDirections(Vector2 dir)
{
    return lockedDirectionsTable_.at(dir);
}

void TilePatternDescriptor::SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections)
{
    lockedDirectionsTable_[dir] = GenerateLockedDirections(directions_, exclusiveDirections);
}

void TilePatternDescriptor::SetLockedDirections(Vector2 dir, const Directions& lockedDirections)
{
    lockedDirectionsTable_[dir] = lockedDirections;
}

// Private

DirectionsTable TilePatternDescriptor::GenerateDefaultLockedDirectionsTable(const Directions& directions)
{
    DirectionsTable lockedDirectionsTable;

    for(const auto dir : directions)
    {
        // The intention is to remove loops
        Vector2 neg = -dir;
        Directions lockedDirections = GenerateLockedDirections(directions, {neg});
        lockedDirectionsTable.insert({dir, lockedDirections});
    }

    return lockedDirectionsTable;
}

DirectionsTable TilePatternDescriptor::GenerateLockedDirectionsTable(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    DirectionsTable lockedDirectionsTable;

    for(const auto pair : exclusiveDirectionsTable)
    {
        auto dir = pair.first;
        auto exclusiveDirections = pair.second;

        auto lockedDirectionsForDir = GenerateLockedDirections(directions, exclusiveDirections);
        lockedDirectionsTable.insert({dir, lockedDirectionsForDir});
    }

    return lockedDirectionsTable;
}

Directions TilePatternDescriptor::GenerateLockedDirections(const Directions& directions, 
    const Directions& exclusiveDirections)
{
    Directions lockedDirections = directions;
    for(const auto exclusiveDir : exclusiveDirections)
        VectorUtils::RemoveByValue(lockedDirections, exclusiveDir);

    return lockedDirections;
}

TilePatternIPtr TilePatternDescriptor::DoCalculateTilePattern(Vector2 origin, 
    std::optional<Vector2> destination, const TilePatternConstraints& constraints)
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
        auto discoverDirections = GetDiscoverDirections(node);
        auto neighbours = tg.DiscoverNeighbours(node->pos, discoverDirections, constraints);
        for(const auto& nei : neighbours)
        {
            auto sharedNei = nei.lock();

            // Check if accumulated cost to this neighbour is lower than previous
            int neiCost = constraints.GetTileCost(sharedNei->pos);
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

Directions TilePatternDescriptor::GetDiscoverDirections(TileNodePtr tileNode)
{
    auto directions = GetDirections();

    auto sTileNode = tileNode.lock();

    auto lowest = [](TileNodePtr a, TileNodePtr b) {
            return a.lock()->cost < b.lock()->cost;
        };
    auto neighbour = sTileNode->GetNeighbourBySortCriteria(lowest);
    if(auto sNeigbour = neighbour.lock())
    {
        Vector2 movement = sTileNode->pos - sNeigbour->pos;
        directions = GetLockedDirections(movement);
    }

    return directions;
}