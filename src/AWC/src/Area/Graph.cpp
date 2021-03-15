#include <AWC/Area/Graph.h>
#include <AWC/AWCException.h>

#include <iostream>

// Graph

Graph::Graph(Graph& tg) : nodes_{tg.nodes_}
{
}

Graph::Graph(Graph&& other) : nodes_{std::move(other.nodes_)}
{
#ifdef _DEBUG
    moved = true;
#endif
}

NodePtr Graph::CreateNode(const Vector2 pos, const unsigned int cost)
{
    std::shared_ptr<Node> mapNode;
    if(!NodeExists(pos))
    {
        mapNode = std::shared_ptr<Node>(new Node{pos, cost});
        nodes_.insert({pos, mapNode});
    }
    else
        throw AWCAlreadyExistingIndexException("Graph::CreateNode", pos);;

    return mapNode;
}

NodePtr Graph::CreateNode(const Vector2 pos, const unsigned int cost, const Vector2 nei)
{
    auto mapNode = CreateNode(pos, cost);
    SetNeighbour(pos, nei);

    return mapNode;
}

std::vector<Vector2> Graph::GetNodesPos() const
{
    std::vector<Vector2> nodesPos;
    for(auto pair : nodes_)
        nodesPos.push_back(pair.first);

    return nodesPos;
}

NodePtr Graph::GetNode(Vector2 pos) const
{
    NodePtr mapNode;
    if(NodeExists(pos))
        mapNode = nodes_.at(pos);
    else
        throw AWCNoExistingIndexException("Graph::GetNode", pos);

    return mapNode;
}

bool Graph::NodeExists(Vector2 pos) const
{
    return nodes_.find(pos) != nodes_.end();
}

void Graph::SetNeighbour(Vector2 a, Vector2 b)
{    
    auto nodeA = GetNode(a);
    auto nodeB = GetNode(b);

    nodeA.lock()->AddNeigbour(b, nodeB);
    nodeB.lock()->AddNeigbour(a, nodeA);
}

std::vector<NodePtr> Graph::DiscoverNeighbours(Vector2 pos, const Directions& directions)
{
    std::vector<NodePtr> neighbours;

    auto current = GetNode(pos);
    for(const auto& dir : directions)
    {
        Vector2 tilePos = pos + dir;

        NodePtr neighbour;
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