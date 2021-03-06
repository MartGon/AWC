#pragma once

#include <Utils.h>

#include <AWC/AWCException.h>

class TilePatternI
{
public:
    virtual ~TilePatternI() {};

    virtual unsigned int GetTileCost(Vector2 dest) const = 0;
    virtual bool IsTileInPattern(Vector2 dest) const = 0;
    virtual std::vector<Vector2> GetPathToTile(Vector2 dest) const = 0;
    virtual std::vector<Vector2> GetTilesPosInPattern() const = 0;
    virtual Vector2 GetOrigin() const = 0;
};