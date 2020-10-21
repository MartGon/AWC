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

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minRange;
    std::vector<Vector2> GetPathToTile(Vector2 dest);
    std::vector<Vector2> GetTilesPosInPattern();
    Vector2 GetOrigin();

private:
    TilePattern(Vector2 origin, TileGraph mg, unsigned int maxRange, unsigned int minRange);

    bool IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange = 0);
    bool IsTileInRange(Vector2 dest);

    Vector2 origin_;
    TileGraph tg_;
    unsigned int minRange_;
    unsigned int maxRange_;
};