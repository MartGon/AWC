#include <AWC/Map.h>
#include <AWC/TileType.h>
#include <AWC/Exception.h>

#include <sstream>

// Public interface

Map::Map(int x, int y) : x_{x}, y_{y}
{
    // TODO: Should change to not use std::vector<std::vector<T>>. [x + y * xSize]
    units_.resize(x);
    for(int i = 0; i < x; i++)
        units_[i] = std::vector<UnitPtr>{(long unsigned int)y};
    
    tiles_.resize(x);
    for(int i = 0; i < x; i++)
        tiles_[i] = std::vector<TilePtr>{(long unsigned int)y};
    
}

int Map::GetWidth() const
{
    return x_;
}

int Map::GetHeight() const
{
    return y_;
}

Vector2 Map::GetSize() const
{
    return Vector2{x_, y_};
}

void Map::AddUnit(int x, int y, UnitPtr unit)
{
    if(IsPositionFree(x, y))
        units_[x][y] = unit;
    else
        throw AWC::AlreadyExistingIndexException("Map::AddUnit", x, y);
}

void Map::AddUnit(Vector2 pos, UnitPtr unit)
{
    AddUnit(pos.x, pos.y, unit);
}

const UnitPtr Map::GetUnit(int x, int y) const
{
    UnitPtr unit = nullptr;
    if(IsPositionValid(x, y))
        unit = units_[x][y];
    else
        throw AWC::NoExistingIndexException("Map::GetUnit", x, y);

    return unit;
}

const UnitPtr Map::GetUnit(Vector2 pos) const
{
    return GetUnit(pos.x, pos.y);
}

void Map::RemoveUnit(int x, int y)
{
    if(IsPositionValid(x, y))
        units_[x][y] = UnitPtr{nullptr};
    else
        throw AWC::NoExistingIndexException("Map::RemoveUnit", x, y);
}

void Map::RemoveUnit(Vector2 pos)
{
    RemoveUnit(pos.x, pos.y);
}

void Map::SetTile(int x, int y, TilePtr tile)
{
    if(IsPositionValid(x, y))
        tiles_[x][y] = tile;
    else
        throw AWC::NoExistingIndexException("Map::SetTile", x, y);
}

void Map::SetTile(Vector2 pos, TilePtr tile)
{
    SetTile(pos.x, pos.y, tile);
}

const TilePtr Map::GetTile(int x, int y) const
{
    TilePtr tile = nullptr;
    if(IsPositionValid(x, y))
        tile = tiles_[x][y];
    else
        throw AWC::NoExistingIndexException("Map::GetTile", x, y);

    return tile;
}

const TilePtr Map::GetTile(Vector2 pos) const
{
    return GetTile(pos.x, pos.y);
}

// Private methods

bool Map::IsPositionFree(int x, int y) const
{
    return GetUnit(x, y) == nullptr;
}

bool Map::IsPositionFree(Vector2 pos) const
{
    return IsPositionFree(pos.x, pos.y);
}

bool Map::IsPositionValid(int x, int y) const
{
    return x >= 0 && x < x_ && y >= 0 && y < y_;
}

bool Map::IsPositionValid(Vector2 pos) const
{
    return IsPositionValid(pos.x, pos.y);
}

// MapUtils

void MapUtils::FillMap(Map& map, TileType& tileType)
{
    for(auto i = 0; i < map.GetWidth(); i++)
    {
        for(auto j = 0; j < map.GetHeight(); j++)
        {
            map.SetTile(i, j, tileType.CreateTile());
        }
    }
}