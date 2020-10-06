#include <AWC/TileNode.h>

TileNode::TileNode(const Vector2 pos, const int cost) : pos{pos}, cost{cost}, steps{this->cost}, visited{false}
{

}

void TileNode::AddNeigbour(Vector2 pos, std::weak_ptr<TileNode> neighbour)
{
    if(!NeighbourExists(pos))
        neighbours_.insert({pos, neighbour});
    else
        throw MapNodeAlreadyExistingNeigbour(pos);
}

std::weak_ptr<TileNode> TileNode::GetNeighbour(Vector2 pos)
{
    std::weak_ptr<TileNode> nei;
    if(NeighbourExists(pos))
        nei = neighbours_[pos];
    else
        throw TileNodeNoExistingNeighbour(pos);

    return nei;
}

std::vector<std::weak_ptr<TileNode>> TileNode::GetNeighbours()
{
    std::vector<std::weak_ptr<TileNode>> neighbours;
    for(auto nei : neighbours_)
        neighbours.push_back(nei.second);

    return neighbours;
}

bool TileNode::NeighbourExists(Vector2 pos)
{
    return neighbours_.find(pos) != neighbours_.end();
}

// Exceptions

TileNodeException::TileNodeException(const std::string& msg, Vector2 pos) : msg_{msg}, pos{pos}
{
}

const char* TileNodeException::what() const noexcept
{
    return msg_.c_str();
}
