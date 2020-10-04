#include <AWC/MapGraph.h>

// MapNode

MapNode::MapNode(const Vector2 pos, const int cost) : pos{pos}, cost{cost}, steps{this->cost}, visited{false}
{

}

void MapNode::AddNeigbour(Vector2 pos, std::weak_ptr<MapNode> neighbour)
{
    if(!NeighbourExists(pos))
        neighbours_.insert({pos, neighbour});
    else
        throw MapNodeAlreadyExistingNeigbour(pos);
}

std::weak_ptr<MapNode> MapNode::GetNeighbour(Vector2 pos)
{
    std::weak_ptr<MapNode> nei;
    if(NeighbourExists(pos))
        nei = neighbours_[pos];
    else
        throw MapNodeNoExistingNeighbour(pos);

    return nei;
}

std::vector<std::weak_ptr<MapNode>> MapNode::GetNeighbours()
{
    std::vector<std::weak_ptr<MapNode>> neighbours;
    for(auto nei : neighbours_)
        neighbours.push_back(nei.second);

    return neighbours;
}

bool MapNode::NeighbourExists(Vector2 pos)
{
    return neighbours_.find(pos) != neighbours_.end();
}

// Exceptions

MapNodeException::MapNodeException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{
}

const char* MapNodeException::what() const noexcept
{
    return msg_.c_str();
}

// MapGraph

std::weak_ptr<MapNode> MapGraph::CreateNode(const Vector2 pos, const int cost)
{
    std::shared_ptr<MapNode> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<MapNode>( new MapNode{pos, cost});
        nodes_.insert({pos, mapNode});
    }
    else
        throw MapGraphAlreadyExistingNode(pos);

    return mapNode;
}

std::weak_ptr<MapNode> MapGraph::GetNode(Vector2 pos)
{
    std::weak_ptr<MapNode> mapNode;
    if(NodeExists(pos))
        mapNode = nodes_[pos];
    else
        throw MapGraphNoExistingNode(pos);

    return mapNode;
}

bool MapGraph::NodeExists(Vector2 pos)
{
    return nodes_.find(pos) != nodes_.end();
}

void MapGraph::SetNeighbour(Vector2 a, Vector2 b)
{    
    auto nodeA = GetNode(a);
    auto nodeB = GetNode(b);

    nodeA.lock()->AddNeigbour(b, nodeB);
    nodeB.lock()->AddNeigbour(a, nodeA);
}

// Exceptions

MapGraphException::MapGraphException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{

}

const char* MapGraphException::what() const noexcept
{
    return msg_.c_str();
}