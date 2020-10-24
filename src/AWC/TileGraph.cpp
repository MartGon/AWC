#include <AWC/TileGraph.h>
#include <AWC/AWCException.h>

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

TileNodePtr TileGraph::CreateNode(const Vector2 pos, const unsigned int cost)
{
    std::shared_ptr<TileNode> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<TileNode>(new TileNode{pos, cost});
        nodes_.insert({pos, mapNode});
    }
    else
        throw AWCAlreadyExistingIndexException("TileGraph::CreateNode", pos);;

    return mapNode;
}

TileNodePtr TileGraph::CreateNode(const Vector2 pos, const unsigned int cost, const Vector2 nei)
{
    auto mapNode = CreateNode(pos, cost);
    SetNeighbour(pos, nei);

    return mapNode;
}

std::vector<Vector2> TileGraph::GetNodesPos() const
{
    std::vector<Vector2> nodesPos;
    for(auto pair : nodes_)
        nodesPos.push_back(pair.first);

    return nodesPos;
}

TileNodePtr TileGraph::GetNode(Vector2 pos) const
{
    TileNodePtr mapNode;
    if(NodeExists(pos))
        mapNode = nodes_.at(pos);
    else
        throw AWCNoExistingIndexException("TileGraph::GetNode", pos);

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

std::vector<TileNodePtr> TileGraph::DiscoverNeighbours(Vector2 pos, const Directions& directions)
{
    std::vector<TileNodePtr> neighbours;

    auto current = GetNode(pos);
    for(const auto& dir : directions)
    {
        Vector2 tilePos = pos + dir;

        TileNodePtr neighbour;
        if(NodeExists(tilePos))
            neighbour = GetNode(tilePos);
        else
        {
            neighbour = CreateNode(tilePos, std::numeric_limits<unsigned int>::max(), pos);
        }

        neighbours.push_back(neighbour);
    }

    return neighbours;
}