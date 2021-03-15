#include <AWC/Area/Node.h>
#include <AWC/AWCException.h>

Node::Node(const Vector2 pos, const unsigned int cost) : pos{pos}, cost{cost}
{

}

void Node::AddNeigbour(Vector2 pos, NodePtr neighbour)
{
    if(!NeighbourExists(pos))
        neighbours_.insert({pos, neighbour});
    else
        throw AWCAlreadyExistingIndexException("Node::AddNeighbour", pos);
}

NodePtr Node::GetNeighbour(Vector2 pos)
{
    NodePtr nei;
    if(NeighbourExists(pos))
        nei = neighbours_[pos];
    else
        throw AWCNoExistingIndexException("Node::GetNeighbour", pos);

    return nei;
}

std::vector<NodePtr> Node::GetNeighbours()
{
    std::vector<NodePtr> neighbours;
    for(auto nei : neighbours_)
        neighbours.push_back(nei.second);

    return neighbours;
}

bool Node::NeighbourExists(Vector2 pos)
{
    return neighbours_.find(pos) != neighbours_.end();
}

NodePtr Node::GetNeighbourBySortCriteria(std::function<bool(NodePtr a, NodePtr b)> sortCriteria)
{
    NodePtr match = std::shared_ptr<Node>{nullptr};

    auto neighbours = GetNeighbours();
    if(!neighbours.empty())
    {
        std::sort(neighbours.begin(), neighbours.end(), sortCriteria);
        match = neighbours.front();
    }

    return match;
}
