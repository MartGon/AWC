#pragma once

#include <AWC/AWCusing.h>
#include <AWC/TilePattern/TilePatternI.h>
#include <AWC/TilePattern/TilePattern.h>

#include <memory>

class TilePatternComp : public TilePatternI
{
public:
    TilePatternComp(TilePatternIPtr a, TilePatternIPtr b);

    virtual Vector2 GetOrigin() const override;

protected:
    TilePatternIPtr a_;
    TilePatternIPtr b_;
};

class TilePatternUnion : public TilePatternComp
{
public:
    TilePatternUnion(TilePatternIPtr a, TilePatternIPtr b);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool IsTileInPattern(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetTilesPosInPattern() const override;
};

class TilePatternDiff : public TilePatternComp
{
public:
    TilePatternDiff(TilePatternIPtr left, TilePatternIPtr right);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool IsTileInPattern(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetTilesPosInPattern() const override;
};

class TilePatternIntersect : public TilePatternComp
{
public:
    TilePatternIntersect(TilePatternIPtr a, TilePatternIPtr b);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool IsTileInPattern(Vector2 dest) const override;

    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetTilesPosInPattern() const override;
};