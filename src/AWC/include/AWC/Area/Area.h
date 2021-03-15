#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Area/Graph.h>
#include <AWC/Area/AreaI.h>
#include <AWC/Range.h>

#include <vector>

class Area : public AreaI
{
friend class AreaDesc;

public:

    unsigned int GetTileCost(Vector2 dest) const override;
    bool ContainsTile(Vector2 dest) const override;

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minRange;
    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetContainedTiles() const override;
    Vector2 GetOrigin() const override;

private:

    Area(Vector2 origin, Graph mg, unsigned int maxRange, unsigned int minRange);
    Area(Vector2 origin, Graph mg, Range range);

    bool IsTileInRange(Vector2 dest, unsigned int maxRange, unsigned int minRange = 0) const;
    bool IsTileInRange(Vector2 dest) const;

    Vector2 origin_;
    Graph tg_;
    Range range_;
};