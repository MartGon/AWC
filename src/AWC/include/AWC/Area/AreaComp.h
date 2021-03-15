#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Area/AreaI.h>
#include <AWC/Area/Area.h>

#include <memory>

class AreaComp : public AreaI
{
public:
    AreaComp(AreaIPtr a, AreaIPtr b);

    virtual Vector2 GetOrigin() const override;

protected:
    AreaIPtr a_;
    AreaIPtr b_;
};

class AreaUnion : public AreaComp
{
public:
    AreaUnion(AreaIPtr a, AreaIPtr b);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool ContainsTile(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetContainedTiles() const override;
};

class AreaDiff : public AreaComp
{
public:
    AreaDiff(AreaIPtr left, AreaIPtr right);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool ContainsTile(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetContainedTiles() const override;
};

class AreaIntersect : public AreaComp
{
public:
    AreaIntersect(AreaIPtr a, AreaIPtr b);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool ContainsTile(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetContainedTiles() const override;
};