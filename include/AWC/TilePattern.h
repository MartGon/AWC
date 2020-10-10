#pragma once

#include <AWC/AWCfwd.h>

#include <AWC/TileGraph.h>
#include <Utils/Vector2.h>

#include <vector>

class TilePattern
{
friend class TilePatternDescriptor;

public:
    unsigned int GetTileCost(Vector2 dest);
    bool IsTileInPattern(Vector2 dest);
    bool IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange = 0);

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minSteps;
    std::vector<Vector2> GetPathToTile(Vector2 dest);
    std::vector<Vector2> GetTilePosInRange();
    Vector2 GetOrigin();

private:
    TilePattern(Vector2 origin, TileGraph mg);

    Vector2 origin_;
    TileGraph tg_;
};