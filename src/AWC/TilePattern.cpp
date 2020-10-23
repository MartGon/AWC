#include <AWC/TilePattern.h>

TilePattern::TilePattern(Vector2 origin, TileGraph tg, unsigned int maxRange, unsigned int minRange) : 
    origin_{origin}, tg_{tg}, maxRange_{maxRange}, minRange_{minRange}
{
    
}

unsigned int TilePattern::GetTileCost(Vector2 dest) const
{
    return tg_.GetNode(dest).lock()->cost;
}

bool TilePattern::IsTileInPattern(Vector2 dest) const
{
    return IsTileInRange(dest);
}

std::vector<Vector2> TilePattern::GetPathToTile(Vector2 dest) const
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

std::vector<Vector2> TilePattern::GetTilesPosInPattern() const
{
    return tg_.GetNodesPos();
}

Vector2 TilePattern::GetOrigin() const
{
    return origin_;
}

// private

bool TilePattern::IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange) const
{
    bool isInPattern = false;
    if(tg_.NodeExists(dest))
    {
        auto cost = tg_.GetNode(dest).lock()->cost;
        isInPattern = cost <= maxRange && cost >= minRange;
    }

    return isInPattern; 
}

bool TilePattern::IsTileInRange(Vector2 dest) const
{
    return IsTileInRange(dest, maxRange_, minRange_);
}