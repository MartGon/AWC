#include <AWC/TilePatternDescriptor.h>
#include <AWC/Map.h>
#include <AWC/TileGraph.h>
#include <Utils/STLUtils.h>

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
    int x, int y, const std::vector<Vector2>& directions)
{
    std::vector<std::weak_ptr<TileNode>> neighbours;

    Vector2 pos{x, y};
    auto current = mg.GetNode({x, y});
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
                neighbour = mg.CreateNode(tilePos, -1, pos);
            }

            neighbours.push_back(neighbour);
        }
    }

    return neighbours;
}

// Exceptions

TilePatternDescriptorException::TilePatternDescriptorException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{
}

const char* TilePatternDescriptorException::what() const noexcept
{
    return msg_.c_str();
}