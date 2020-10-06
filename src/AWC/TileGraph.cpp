#include <AWC/TileGraph.h>

#include <iostream>

// TileGraph

TileGraph::TileGraph(TileGraph&& other)
{
    nodes_ = std::move(other.nodes_);
#ifdef _DEBUG
    moved = true;
#endif
}

std::weak_ptr<TileNode> TileGraph::CreateNode(const Vector2 pos, const int cost)
{
    std::shared_ptr<TileNode> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<TileNode>( new TileNode{pos, cost});
        nodes_.insert({pos, mapNode});
    }
    else
        throw TileGraphAlreadyExistingNode(pos);

    return mapNode;
}

std::weak_ptr<TileNode> TileGraph::GetNode(Vector2 pos)
{
    std::weak_ptr<TileNode> mapNode;
    if(NodeExists(pos))
        mapNode = nodes_[pos];
    else
        throw TileGraphNoExistingNode(pos);

    return mapNode;
}

bool TileGraph::NodeExists(Vector2 pos)
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