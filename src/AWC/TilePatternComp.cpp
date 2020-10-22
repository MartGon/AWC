#include <AWC/TilePatternComp.h>

#include <Utils/STLUtils.h>

#include <algorithm>

TilePatternUnion::TilePatternUnion(TilePatternIPtr a, TilePatternIPtr b) : a_{a}, b_{b}
{

}

unsigned int TilePatternUnion::GetTileCost(Vector2 dest) const
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
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
    std::vector<Vector2> result;

    for(auto tile : tilesA)
        if(!VectorUtils::IsInside(result, tile))
            result.push_back(tile);

    for(auto tile : tilesB)
        if(!VectorUtils::IsInside(result, tile))
            result.push_back(tile);

    return result;
}

Vector2 TilePatternUnion::GetOrigin() const
{
    return a_->GetOrigin();
}