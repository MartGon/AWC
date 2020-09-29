#include <AWC/MapGraph.h>

// MapNode

MapNode::MapNode(const Vector2 pos, const int cost) : pos{pos}, cost{cost}
{

}

void MapNode::AddNeigbour(Vector2 pos, std::weak_ptr<MapNode> neighbour)
{
    neighbours_.insert({pos, neighbour});
}

std::weak_ptr<MapNode> MapNode::GetNeighbour(Vector2 pos)
{
    std::weak_ptr<MapNode> nei;
    if(neighbours_.find(pos) != neighbours_.end())
        nei = neighbours_[pos];

    return nei;
}

std::vector<std::weak_ptr<MapNode>> MapNode::GetNeighbours()
{
    std::vector<std::weak_ptr<MapNode>> neighbours;
    for(auto nei : neighbours_)
        neighbours.push_back(nei.second);

    return neighbours;
}

// MapGraph

// TODO: Throw exception if already exists
std::weak_ptr<MapNode> MapGraph::CreateNode(const Vector2 pos, const int cost)
{
    std::shared_ptr<MapNode> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<MapNode>( new MapNode{pos, cost});
        nodes_.insert({pos, mapNode});
    }

    return mapNode;
}

std::weak_ptr<MapNode> MapGraph::GetNode(Vector2 pos)
{
    std::weak_ptr<MapNode> mapNode;
    if(NodeExists(pos))
        mapNode = nodes_[pos];

    return mapNode;
}

bool MapGraph::NodeExists(Vector2 pos)
{
    return nodes_.find(pos) != nodes_.end();
}