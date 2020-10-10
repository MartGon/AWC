#include <AWC/TilePattern.h>
#include <AWC/TilePatternDescriptor.h>
#include <AWC/TilePatternConstraints.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TileGraph.h>
#include <Utils/STLUtils.h>

#include <limits>
#include <queue>
#include <functional>

// Public

// Factory methods
TilePatternDescriptor TilePatternDescriptor::CreateTilePatternDescriptorByLockedDirectionsMap(const std::vector<Vector2>& directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& lockedDirectionsMap)
{
    return TilePatternDescriptor{directions, lockedDirectionsMap};
}

TilePatternDescriptor TilePatternDescriptor::CreateTilePatternDescriptorByExclusiveDirectionsMap(const std::vector<Vector2>& directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& exclusiveDirectionsMap)
{
    return TilePatternDescriptor{directions, GenerateLockedDirectionsMap(directions, exclusiveDirectionsMap)};
}

// Constructors
TilePatternDescriptor::TilePatternDescriptor(const std::vector<Vector2>& directions) : 
    directions_{directions}, lockedDirectionsMap_{GenerateDefaultLockedDirectionsMap(directions)}
{

}

TilePatternDescriptor::TilePatternDescriptor(const std::vector<Vector2>& directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& lockedDirectionsMap) : 
    directions_{directions}, lockedDirectionsMap_{lockedDirectionsMap}
{

}

// Methods

std::vector<Vector2> TilePatternDescriptor::GetDirections()
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
        throw TilePatternDescriptorAlreadyExistingDirection(dir);
}

void TilePatternDescriptor::RemoveDirection(Vector2 dir)
{
    if(IsDirection(dir))
    {
        VectorUtils::RemoveByValue(directions_, dir);
        lockedDirectionsMap_.erase(dir);
    }
    else
        throw TilePatternDescriptorNoExistingDirection(dir);

}

std::vector<Vector2> TilePatternDescriptor::GetLockedDirections(Vector2 dir)
{
    return lockedDirectionsMap_.at(dir);
}

void TilePatternDescriptor::SetExclusiveDirections(Vector2 dir, const std::vector<Vector2>& exclusiveDirections)
{
    lockedDirectionsMap_[dir] = GenerateLockedDirections(directions_, exclusiveDirections);
}

void TilePatternDescriptor::SetLockedDirections(Vector2 dir, const std::vector<Vector2>& lockedDirections)
{
    lockedDirectionsMap_[dir] = lockedDirections;
}

// Main methods

std::shared_ptr<TilePattern> TilePatternDescriptor::CalculateTilePattern(Vector2 origin, TilePatternConstraints constraints)
{
    // Create mapGraph
    TileGraph tg;
    auto originNode = tg.CreateNode(origin, 0);

    // Create prioQueue
    // PrioQueue pop values in reverse order
    auto greater = [](std::weak_ptr<TileNode> a, std::weak_ptr<TileNode> b) { return a.lock()->cost > b.lock()->cost;};
    std::priority_queue<std::weak_ptr<TileNode>, std::vector<std::weak_ptr<TileNode>>, decltype(greater)> prioQueue{greater};
    prioQueue.push(originNode);

    while(!prioQueue.empty())
    {
        // Pop first member
        auto node = prioQueue.top().lock();
        prioQueue.pop();

        // Get its neighbours
        auto discoverDirections = GetDiscoverDirections(node);
        auto neighbours = DiscoverNeighbours(constraints.map, tg, node->pos, discoverDirections);
        for(const auto& nei : neighbours)
        {
            auto sharedNei = nei.lock();

            // Check if accumulated cost to this neighbour is lower than previous
            if(auto tile = constraints.map.GetTile(sharedNei->pos.x, sharedNei->pos.y))
            {
                int neiCost = constraints.tileCostTable.GetCost(tile->GetId());
                int calculatedCost = node->cost + neiCost;
                if(calculatedCost < sharedNei->cost && calculatedCost <= constraints.maxRange)
                {
                    // Push it to queue if that's the case
                    sharedNei->cost = calculatedCost;
                    prioQueue.push(nei);
                }
            }
        }
    }

    // Create TilePattern
    auto tp = std::shared_ptr<TilePattern>(new TilePattern{origin, tg});

    return tp;
}

std::shared_ptr<TilePattern> TilePatternDescriptor::CalculateTilePattern(Vector2 origin, Vector2 destination, TilePatternConstraints constraints)
{

}

// Private

std::unordered_map<Vector2, std::vector<Vector2>> TilePatternDescriptor::GenerateDefaultLockedDirectionsMap(const std::vector<Vector2>& directions)
{
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap;

    for(const auto dir : directions)
    {
        // The intention is to remove loops
        Vector2 neg = -dir;
        std::vector<Vector2> lockedDirections = GenerateLockedDirections(directions, {neg});
        lockedDirectionsMap.insert({dir, lockedDirections});
    }

    return lockedDirectionsMap;
}

std::unordered_map<Vector2, std::vector<Vector2>> TilePatternDescriptor::GenerateLockedDirectionsMap(const std::vector<Vector2>& directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& exclusiveDirectionsMap)
{
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap;

    for(const auto pair : exclusiveDirectionsMap)
    {
        auto dir = pair.first;
        auto exclusiveDirections = pair.second;

        auto lockedDirectionsForDir = GenerateLockedDirections(directions, exclusiveDirections);
        lockedDirectionsMap.insert({dir, lockedDirectionsForDir});
    }

    return lockedDirectionsMap;
}

std::vector<Vector2> TilePatternDescriptor::GenerateLockedDirections(const std::vector<Vector2>& directions, 
    const std::vector<Vector2>& exclusiveDirections)
{
    std::vector<Vector2> lockedDirections = directions;
    for(const auto exclusiveDir : exclusiveDirections)
        VectorUtils::RemoveByValue(lockedDirections, exclusiveDir);

    return lockedDirections;
}

std::vector<std::weak_ptr<TileNode>> TilePatternDescriptor::DiscoverNeighbours(const Map& map, TileGraph& mg, 
    Vector2 pos, const std::vector<Vector2>& directions)
{
    std::vector<std::weak_ptr<TileNode>> neighbours;

    auto current = mg.GetNode(pos);
    for(const auto& dir : directions)
    {
        Vector2 tilePos = pos + dir;
    
        if(map.IsPositionValid(tilePos.x, tilePos.y))
        {
            std::weak_ptr<TileNode> neighbour;
            if(mg.NodeExists(tilePos))
                neighbour = mg.GetNode(tilePos);
            else
            {
                neighbour = mg.CreateNode(tilePos, std::numeric_limits<unsigned int>::max(), pos);
            }

            neighbours.push_back(neighbour);
        }
    }

    return neighbours;
}

std::vector<Vector2> TilePatternDescriptor::GetDiscoverDirections(std::weak_ptr<TileNode> tileNode)
{
    auto directions = GetDirections();

    auto sTileNode = tileNode.lock();

    auto lowest = [](std::weak_ptr<TileNode> a, std::weak_ptr<TileNode> b) {
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

// Exceptions

TilePatternDescriptorException::TilePatternDescriptorException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{
}

const char* TilePatternDescriptorException::what() const noexcept
{
    return msg_.c_str();
}