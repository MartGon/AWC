#pragma once

#include <memory>
#include <vector>
#include <exception>
#include <string>

#include <AWC/AWCusing.h>
#include <AWC/Unit/Unit.h>
#include <AWC/Tile.h>

#include <Utils.h>

class Map
{
public:
    Map(int x, int y);

    int GetWidth() const;
    int GetHeight() const;
    Vector2 GetSize() const;

    void AddUnit(int x, int y, UnitPtr unit);
    void AddUnit(Vector2 pos, UnitPtr unit);

    const UnitPtr GetUnit(int x, int y) const;
    const UnitPtr GetUnit(Vector2 pos) const;

    void RemoveUnit(int x, int y);
    void RemoveUnit(Vector2 pos);

    // Tiles

    void SetTile(int x, int y, TilePtr tile);
    void SetTile(Vector2 pos, TilePtr tile);

    const TilePtr GetTile(int x, int y) const;
    const TilePtr GetTile(Vector2 pos) const;

    bool IsPositionFree(int x, int y) const;
    bool IsPositionFree(Vector2 pos) const;

    bool IsPositionValid(int x, int y) const;
    bool IsPositionValid(Vector2 pos) const;

private:

    int x_;
    int y_;

    std::vector<std::vector<UnitPtr>> units_;
    std::vector<std::vector<TilePtr>> tiles_; 
};

// MapUtils

namespace MapUtils
{
    void FillMap(Map& map, TileType& tileType);
}