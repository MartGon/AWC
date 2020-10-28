#pragma once

#include <AWC/AWCfwd.h>

#include <AWC/TileGraph.h>
#include <Utils/Vector2.h>
#include <AWC/TilePatternI.h>

#include <vector>

class TilePattern : public TilePatternI
{
friend class TilePatternDesc;

public:
    unsigned int GetTileCost(Vector2 dest) const override;
    bool IsTileInPattern(Vector2 dest) const override;

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minRange;
    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetTilesPosInPattern() const override;
    Vector2 GetOrigin() const override;

private:
    TilePattern(Vector2 origin, TileGraph mg, unsigned int maxRange, unsigned int minRange);

    bool IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange = 0) const;
    bool IsTileInRange(Vector2 dest) const;

    Vector2 origin_;
    TileGraph tg_;
    unsigned int minRange_;
    unsigned int maxRange_;
};