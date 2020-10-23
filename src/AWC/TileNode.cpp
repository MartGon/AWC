#include <AWC/TileNode.h>
#include <AWC/AWCException.h>

TileNode::TileNode(const Vector2 pos, const unsigned int cost) : pos{pos}, cost{cost}
{

}

void TileNode::AddNeigbour(Vector2 pos, TileNodePtr neighbour)
{
    if(!NeighbourExists(pos))
        neighbours_.insert({pos, neighbour});
    else
        throw AWCAlreadyExistingIndexException("TileNode::AddNeighbour", pos);
}

TileNodePtr TileNode::GetNeighbour(Vector2 pos)
{
    TileNodePtr nei;
    if(NeighbourExists(pos))
        nei = neighbours_[pos];
    else
        throw AWCNoExistingIndexException("TileNode::GetNeighbour", pos);

    return nei;
}

std::vector<TileNodePtr> TileNode::GetNeighbours()
{
    std::vector<TileNodePtr> neighbours;
    for(auto nei : neighbours_)
        neighbours.push_back(nei.second);

    return neighbours;
}

bool TileNode::NeighbourExists(Vector2 pos)
{
    return neighbours_.find(pos) != neighbours_.end();
}

TileNodePtr TileNode::GetNeighbourBySortCriteria(std::function<bool(TileNodePtr a, TileNodePtr b)> sortCriteria)
{
    TileNodePtr match = std::shared_ptr<TileNode>{nullptr};

    auto neighbours = GetNeighbours();
    if(!neighbours.empty())
    {
        std::sort(neighbours.begin(), neighbours.end(), sortCriteria);
        match = neighbours.front();
    }

    return match;
}
