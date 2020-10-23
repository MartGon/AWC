#include <AWC/TilePatternComp.h>

#include <Utils/STLUtils.h>

#include <algorithm>

TilePatternUnion::TilePatternUnion(TilePatternIPtr a, TilePatternIPtr b) : a_{a}, b_{b}
{

}

unsigned int TilePatternUnion::GetTileCost(Vector2 dest) const
{
    unsigned int cost;
    if(a_->IsTileInPattern(dest))
        cost = a_->GetTileCost(dest);
    else if(b_->IsTileInPattern(dest))
        cost = b_->GetTileCost(dest);

    return cost;
}

bool TilePatternUnion::IsTileInPattern(Vector2 dest) const
{
    return a_->IsTileInPattern(dest) || b_->IsTileInPattern(dest);
}

std::vector<Vector2> TilePatternUnion::GetPathToTile(Vector2 dest) const
{
    std::vector<Vector2> path;
    if(a_->IsTileInPattern(dest))
        path = a_->GetPathToTile(dest);
    else if(b_->IsTileInPattern(dest))
        path = b_->GetPathToTile(dest);

    return path;
}

std::vector<Vector2> TilePatternUnion::GetTilesPosInPattern() const
{
    auto tilesA = a_->GetTilesPosInPattern();
    auto tilesB = b_->GetTilesPosInPattern();
    
    return VectorUtils::Union(tilesA, tilesB);
}

Vector2 TilePatternUnion::GetOrigin() const
{
    return a_->GetOrigin();
}