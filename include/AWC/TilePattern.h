#pragma once

#include <AWC/MapGraph.h>
#include <Utils/Vector2.h>

#include <vector>

class TilePattern
{
public:
    TilePattern(Vector2 origin, MapGraph mg);
    bool IsTileInRange(Vector2 dest);

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minSteps;
    std::vector<Vector2> GetPathToTile(Vector2 dest);
    std::vector<Vector2> GetTilePosInRange();
    Vector2 GetOrigin();

private:
    Vector2 origin_;
    MapGraph mg_;
};