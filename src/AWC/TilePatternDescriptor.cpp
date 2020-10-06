#include <AWC/TilePatternDescriptor.h>

#include <queue>
#include <functional>

#include <AWC/Map.h>
#include <AWC/TileGraph.h>

TilePatternDescriptor::TilePatternDescriptor(int minDist, int maxDist, const TilePatternDescriptorType& type) : minDist_{minDist}, maxDist_{maxDist}, type_{type}
{

}

std::vector<std::weak_ptr<TileNode>> TilePatternDescriptor::DiscoverNeighbours(const Map& map, TileGraph& mg, int x, int y, const std::vector<Vector2>& directions)
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