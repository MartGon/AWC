#include <AWC/Area/AreaComp.h>

#include <Utils.h>

#include <algorithm>

// Comp
AreaComp::AreaComp(AreaIPtr a, AreaIPtr b) : a_{a}, b_{b}
{

}

Vector2 AreaComp::GetOrigin() const
{
    return a_->GetOrigin();
}

// Union

AreaUnion::AreaUnion(AreaIPtr a, AreaIPtr b) : AreaComp(a, b)
{

}

unsigned int AreaUnion::GetTileCost(Vector2 dest) const
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
    if(a_->ContainsTile(dest))
        cost = cost, a_->GetTileCost(dest);
    if(b_->ContainsTile(dest))
        cost = std::min(cost, b_->GetTileCost(dest));

    return cost;
}

bool AreaUnion::ContainsTile(Vector2 dest) const
{
    return a_->ContainsTile(dest) || b_->ContainsTile(dest);
}

std::vector<Vector2> AreaUnion::GetPathToTile(Vector2 dest) const
{
    std::vector<Vector2> path;
    if(a_->ContainsTile(dest))
        path = a_->GetPathToTile(dest);
    else if(b_->ContainsTile(dest))
        path = b_->GetPathToTile(dest);

    return path;
}

std::vector<Vector2> AreaUnion::GetContainedTiles() const
{
    auto tilesA = a_->GetContainedTiles();
    auto tilesB = b_->GetContainedTiles();
    
    return VectorUtils::Union(tilesA, tilesB);
}

// Diff

AreaDiff::AreaDiff(AreaIPtr left, AreaIPtr right) : AreaComp(left, right)
{

}

unsigned int AreaDiff::GetTileCost(Vector2 dest) const
{
    unsigned int cost = std::numeric_limits<unsigned int>::max();
    if(ContainsTile(dest))
        cost = a_->GetTileCost(dest);

    return cost;
}

bool AreaDiff::ContainsTile(Vector2 dest) const
{
    return a_->ContainsTile(dest) && !b_->ContainsTile(dest);
}

// Two options here:
// 1- Return in path tiles that the pattern cannot go directly, but is able to traverse 
// 2- Remove the tiles from the path which the restricting pattern can go to.

std::vector<Vector2> AreaDiff::GetPathToTile(Vector2 dest) const
{
    std::vector<Vector2> path;
    if(ContainsTile(dest))
        path = a_->GetPathToTile(dest);

    return path;
}

std::vector<Vector2> AreaDiff::GetContainedTiles() const
{
    auto tilesA = a_->GetContainedTiles();
    auto tilesB = b_->GetContainedTiles();
    
    return VectorUtils::Diff(tilesA, tilesB);
}

// Intersect

AreaIntersect::AreaIntersect(AreaIPtr a, AreaIPtr b) : AreaComp(a, b)
{

}

unsigned int AreaIntersect::GetTileCost(Vector2 pos) const
{
    uint cost = std::numeric_limits<uint>::max();
    if(ContainsTile(pos))
        cost = std::min(a_->GetTileCost(pos), b_->GetTileCost(pos));
    
    return cost;
}

bool AreaIntersect::ContainsTile(Vector2 pos) const
{
    return a_->ContainsTile(pos) && b_->ContainsTile(pos);
}

std::vector<Vector2> AreaIntersect::GetPathToTile(Vector2 pos) const
{
    std::vector<Vector2> path;
    if(ContainsTile(pos))
    {
        auto pathA = a_->GetPathToTile(pos);
        auto pathB = b_->GetPathToTile(pos);
        path = pathA.size() < pathB.size() ? pathA : pathB;
    }

    return path;
}

std::vector<Vector2> AreaIntersect::GetContainedTiles() const
{
    auto tilesA = a_->GetContainedTiles();
    auto tilesB = b_->GetContainedTiles();
    auto res = VectorUtils::Intersect(tilesA, tilesB);

    return res;
}