#include <AWC/TileGraph.h>

#include <iostream>

// TileGraph

TileGraph::TileGraph(TileGraph& tg) : nodes_{tg.nodes_}
{
}

TileGraph::TileGraph(TileGraph&& other) : nodes_{std::move(other.nodes_)}
{
#ifdef _DEBUG
    moved = true;
#endif
}

std::weak_ptr<TileNode> TileGraph::CreateNode(const Vector2 pos, const unsigned int cost)
{
    std::shared_ptr<TileNode> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<TileNode>(new TileNode{pos, cost});
        nodes_.insert({pos, mapNode});
    }
    else
        throw TileGraphAlreadyExistingNode(pos);

    return mapNode;
}

std::weak_ptr<TileNode> TileGraph::CreateNode(const Vector2 pos, const unsigned int cost, const Vector2 nei)
{
    auto mapNode = CreateNode(pos, cost);
    SetNeighbour(pos, nei);

    return mapNode;
}

std::vector<Vector2> TileGraph::GetNodesPos() const
{
    std::vector<Vector2> nodesPos{nodes_.size()};
    for(auto pair : nodes_)
        nodesPos.push_back(pair.first);

    return nodesPos;
}

std::weak_ptr<TileNode> TileGraph::GetNode(Vector2 pos) const
{
    std::weak_ptr<TileNode> mapNode;
    if(NodeExists(pos))
        mapNode = nodes_.at(pos);
    else
        throw TileGraphNoExistingNode(pos);

    return mapNode;
}

bool TileGraph::NodeExists(Vector2 pos) const
{
    return nodes_.find(pos) != nodes_.end();
}

void TileGraph::SetNeighbour(Vector2 a, Vector2 b)
{    
    auto nodeA = GetNode(a);
    auto nodeB = GetNode(b);

    nodeA.lock()->AddNeigbour(b, nodeB);
    nodeB.lock()->AddNeigbour(a, nodeA);
}

// Exceptions

TileGraphException::TileGraphException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{

}

const char* TileGraphException::what() const noexcept
{
    return msg_.c_str();
}