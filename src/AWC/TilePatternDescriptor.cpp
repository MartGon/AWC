#include <AWC/TilePatternDescriptor.h>
#include <AWC/Map.h>
#include <AWC/TileGraph.h>
#include <Utils/STLUtils.h>

#include <queue>
#include <functional>

// Public

TilePatternDescriptor::TilePatternDescriptor(const std::vector<Vector2>& directions) : 
    directions_{directions}, lockedDirectionsMap_{GenerateDefaultLockedDirections(directions)}
{

}

TilePatternDescriptor::TilePatternDescriptor(std::vector<Vector2> directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& excludedDirections) : 
    directions_{directions}, lockedDirectionsMap_{GenerateLockedDirectionsFromExcluded(directions, excludedDirections)}
{

}

std::vector<Vector2> TilePatternDescriptor::GetLockedDirections(Vector2 dir)
{
    return lockedDirectionsMap_.at(dir);
}

// Private

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
                neighbour = mg.CreateNode(tilePos, -1);
            }

            neighbours.push_back(neighbour);
        }
    }

    return neighbours;
}

std::unordered_map<Vector2, std::vector<Vector2>> TilePatternDescriptor::GenerateDefaultLockedDirections(const std::vector<Vector2>& directions)
{
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap;

    for(const auto dir : directions)
    {
        std::vector<Vector2> lockedDirections = directions;

        // The intention is to remove loops
        Vector2 neg = -dir;
        if(VectorUtils::IsInside(directions, neg))
        {
            VectorUtils::RemoveByValue(lockedDirections, dir);
        }

        lockedDirectionsMap.insert({dir, lockedDirections});
    }

    return lockedDirectionsMap;
}

std::unordered_map<Vector2, std::vector<Vector2>> TilePatternDescriptor::GenerateLockedDirectionsFromExcluded(const std::vector<Vector2>& directions, 
    const std::unordered_map<Vector2, std::vector<Vector2>>& excludedDirections)
{
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap;

    for(const auto pair : excludedDirections)
    {
        auto dir = pair.first;
        auto excludedDirections = pair.second;

        std::vector<Vector2> lockedDirectionsForDir;
        for(const auto validDirection : directions)
        {
            if(!VectorUtils::IsInside(excludedDirections, validDirection))
                lockedDirectionsForDir.push_back(validDirection);
        }

        lockedDirectionsMap.insert({dir, lockedDirectionsForDir});
    }

    return lockedDirectionsMap;
}