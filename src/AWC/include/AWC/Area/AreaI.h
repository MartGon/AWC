#pragma once

#include <Utils.h>

#include <AWC/Exception.h>

class AreaI
{
public:
    virtual ~AreaI() {};

    virtual unsigned int GetTileCost(Vector2 dest) const = 0;
    virtual bool ContainsTile(Vector2 dest) const = 0;
    virtual std::vector<Vector2> GetPathToTile(Vector2 dest) const = 0;
    virtual std::vector<Vector2> GetContainedTiles() const = 0;
    virtual Vector2 GetOrigin() const = 0;
};