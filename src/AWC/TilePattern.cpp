#include <AWC/TilePattern.h>

TilePattern::TilePattern(Vector2 origin, TileGraph tg, unsigned int maxRange, unsigned int minRange) : 
    origin_{origin}, tg_{tg}, maxRange_{maxRange}, minRange_{minRange}
{
    
}

unsigned int TilePattern::GetTileCost(Vector2 dest)
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
    if(TilePattern::IsTileInRange(dest))
        return tg_.GetNode(dest).lock()->cost;
    /*
    else
        throw std::exception()
    */
    
    return cost;
}

bool TilePattern::IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange)
{
    bool isInPattern = false;
    if(tg_.NodeExists(dest))
    {
        auto cost = tg_.GetNode(dest).lock()->cost;
        isInPattern = cost <= maxRange && cost >= minRange;
    }

    return isInPattern; 
}

bool TilePattern::IsTileInRange(Vector2 dest)
{
    return IsTileInRange(dest, maxRange_, minRange_);
}

std::vector<Vector2> TilePattern::GetPathToTile(Vector2 dest)
{
    std::vector<Vector2> path;
    if(IsTileInRange(dest))
    {
        auto node = tg_.GetNode(dest).lock();
        path.push_back(dest);

        while(node->pos != origin_)
        {
            auto lowest = [](std::weak_ptr<TileNode> a, std::weak_ptr<TileNode> b) {
                return a.lock()->cost < b.lock()->cost;
            };
            auto nei = node->GetNeighbourBySortCriteria(lowest);
            if(auto sharedNei = nei.lock())
            {
                path.push_back(sharedNei->pos);
                node = sharedNei;
            }
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}