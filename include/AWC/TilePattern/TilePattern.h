#pragma once

#include <AWC/AWCusing.h>
#include <AWC/TilePattern/TileGraph.h>
#include <AWC/TilePattern/TilePatternI.h>
#include <AWC/Range.h>

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
    TilePattern(Vector2 origin, TileGraph mg, Range range);

    bool IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange = 0) const;
    bool IsTileInRange(Vector2 dest) const;

    Vector2 origin_;
    TileGraph tg_;
    Range range_;
};