#include <AWC/TilePattern.h>

#include <queue>
#include <functional>

#include <AWC/Map.h>
#include <AWC/MapGraph.h>

ManhattanPattern::ManhattanPattern(int minDist, int maxDist) : minDist_{minDist}, maxDist_{maxDist}
{

}

bool ManhattanPattern::IsTileInRange(const Map& map, int originX, int originY, int destX, int destY)
{
    std::vector<Vector2> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    bool inRange = 0;

    // Create mapGraph
    MapGraph mg;
    auto origin = mg.CreateNode({originX, originY}, maxDist_);

    // Create prioQueue
    // PrioQueue pop values in reverse order
    auto greater = [](std::weak_ptr<MapNode> a, std::weak_ptr<MapNode> b) { return a.lock()->steps < b.lock()->steps;};
    std::priority_queue<std::weak_ptr<MapNode>, std::vector<std::weak_ptr<MapNode>>, decltype(greater)> prioQueue{greater};
    prioQueue.push(origin);

    while(!prioQueue.empty())
    {
        // Pop first member
        auto node = prioQueue.top().lock();
        node->visited = true;
        prioQueue.pop();

        // Break if target is the current node;
        if(node->pos == Vector2{destX, destY})
            break;

        // Get its neighbours
        auto neighbours = DiscoverNeighbours(map, mg, node->pos.x, node->pos.y, directions);
        for(const auto& nei : neighbours)
        {
            auto sharedNei = nei.lock();
            int neiCost = 1;

            // Check if steps from current node to neig are greater that previous
            int calculatedSteps = node->steps - neiCost;
            if(calculatedSteps > sharedNei->steps && calculatedSteps >= 0)
            {
                // Push it to queue if that's the case
                sharedNei->steps = calculatedSteps;
                prioQueue.push(nei);
            }
        }
    }

    if(mg.NodeExists({destX, destY}))
    {
        auto targetNode = mg.GetNode({destX, destY}).lock();
        inRange = targetNode->steps <= maxDist_ && targetNode->steps >= 0;
    }

    return inRange;
}

std::vector<std::weak_ptr<MapNode>> ManhattanPattern::DiscoverNeighbours(const Map& map, MapGraph& mg, int x, int y, const std::vector<Vector2>& directions)
{
    std::vector<std::weak_ptr<MapNode>> neighbours;

    Vector2 pos{x, y};
    auto current = mg.GetNode({x, y});
    for(const auto& dir : directions)
    {
        Vector2 tilePos = pos + dir;
        
        if(map.IsPositionValid(tilePos.x, tilePos.y))
        {
            std::weak_ptr<MapNode> neighbour;
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