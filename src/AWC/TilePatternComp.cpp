#include <AWC/TilePatternComp.h>

#include <Utils/STLUtils.h>

#include <algorithm>

// Comp
TilePatternComp::TilePatternComp(TilePatternIPtr a, TilePatternIPtr b) : a_{a}, b_{b}
{

}

Vector2 TilePatternComp::GetOrigin() const
{
    return a_->GetOrigin();
}

// Union

TilePatternUnion::TilePatternUnion(TilePatternIPtr a, TilePatternIPtr b) : TilePatternComp(a, b)
{

}

unsigned int TilePatternUnion::GetTileCost(Vector2 dest) const
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
    if(a_->IsTileInPattern(dest))
        cost = cost, a_->GetTileCost(dest);
    if(b_->IsTileInPattern(dest))
        cost = std::min(cost, b_->GetTileCost(dest));

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

// Diff

TilePatternDiff::TilePatternDiff(TilePatternIPtr left, TilePatternIPtr right) : TilePatternComp(left, right)
{

}

unsigned int TilePatternDiff::GetTileCost(Vector2 dest) const
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
    if(IsTileInPattern(dest))
        cost = a_->GetTileCost(dest);

    return cost;
}

bool TilePatternDiff::IsTileInPattern(Vector2 dest) const
{
    return a_->IsTileInPattern(dest) && !b_->IsTileInPattern(dest);
}

// Two options here:
// 1- Return in path tiles that the pattern cannot go directly, but is able to traverse 
// 2- Remove the tiles from the path which the restricting pattern can go to.

std::vector<Vector2> TilePatternDiff::GetPathToTile(Vector2 dest) const
{
    std::vector<Vector2> path;
    if(IsTileInPattern(dest))
        path = a_->GetPathToTile(dest);

    return path;
}

std::vector<Vector2> TilePatternDiff::GetTilesPosInPattern() const
{
    auto tilesA = a_->GetTilesPosInPattern();
    auto tilesB = b_->GetTilesPosInPattern();
    
    return VectorUtils::Diff(tilesA, tilesB);
}

// Intersect

TilePatternIntersect::TilePatternIntersect(TilePatternIPtr a, TilePatternIPtr b) : TilePatternComp(a, b)
{

}

unsigned int TilePatternIntersect::GetTileCost(Vector2 pos) const
{
    uint cost = std::numeric_limits<uint>::max();
    if(IsTileInPattern(pos))
        cost = std::min(a_->GetTileCost(pos), b_->GetTileCost(pos));
    
    return cost;
}

bool TilePatternIntersect::IsTileInPattern(Vector2 pos) const
{
    return a_->IsTileInPattern(pos) && b_->IsTileInPattern(pos);
}

std::vector<Vector2> TilePatternIntersect::GetPathToTile(Vector2 pos) const
{
    std::vector<Vector2> path;
    if(IsTileInPattern(pos))
    {
        auto pathA = a_->GetPathToTile(pos);
        auto pathB = b_->GetPathToTile(pos);
        path = pathA.size() < pathB.size() ? pathA : pathB;
    }

    return path;
}

std::vector<Vector2> TilePatternIntersect::GetTilesPosInPattern() const
{
    auto tilesA = a_->GetTilesPosInPattern();
    auto tilesB = b_->GetTilesPosInPattern();
    auto res = VectorUtils::Intersect(tilesA, tilesB);

    return res;
}