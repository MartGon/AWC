#include <AWC/Map.h>

#include <sstream>

// Public interface

Map::Map(int x, int y) : x_{x}, y_{y}
{
    // TODO: Should change to not use std::vector<std::vector<T>>. [x + y * xSize]
    units_.resize(x);
    for(int i = 0; i < x; i++)
        units_[i] = std::vector<std::shared_ptr<Unit>>{(long unsigned int)y};
    
    tiles_.resize(x);
    for(int i = 0; i < x; i++)
        tiles_[i] = std::vector<std::shared_ptr<Tile>>{(long unsigned int)y};
    
}

int Map::GetWidht() const
{
    return x_;
}

int Map::GetHeight() const
{
    return y_;
}

void Map::AddUnit(int x, int y, std::shared_ptr<Unit> unit)
{
    if(IsPositionFree(x, y))
        units_[x][y] = unit;
    else
        throw MapInvalidUnitPosition(*this, x, y);
}

const std::shared_ptr<Unit> Map::GetUnit(int x, int y) const
{
    std::shared_ptr<Unit> unit = nullptr;
    if(IsPositionValid(x, y))
        unit = units_[x][y];
    else
        throw MapIndexOutOfBounds(*this, x, y);

    return unit;
}

void Map::RemoveUnit(int x, int y)
{
    if(IsPositionValid(x, y))
        units_[x][y] = std::shared_ptr<Unit>{nullptr};
    else
        throw MapIndexOutOfBounds(*this, x, y);
}

void Map::SetTile(int x, int y, std::shared_ptr<Tile> tile)
{
    // TODO: Previous tile should be destroyed if using raw pointers
    if(IsPositionValid(x, y))
        tiles_[x][y] = tile;
    else
        throw MapIndexOutOfBounds(*this, x, y);
}

const std::shared_ptr<Tile> Map::GetTile(int x, int y) const
{
    std::shared_ptr<Tile> tile = nullptr;
    if(IsPositionValid(x, y))
        tile = tiles_[x][y];
    else
        throw MapIndexOutOfBounds(*this, x, y);

    return tile;
}

// Private methods

bool Map::IsPositionFree(int x, int y) const
{
    return GetUnit(x, y) == nullptr;
}

bool Map::IsPositionValid(int x, int y) const
{
    return x >= 0 && x < x_ && y >= 0 && y < y_;
}

// Exceptions

MapException::MapException(const Map& map) : _map{map}
{

}

const char* MapException::what() const noexcept
{
    return GetErrorMessage().c_str();
}

MapIndexOutOfBounds::MapIndexOutOfBounds(const Map& map, int x, int y) : _x{x}, _y{y}, MapException(map)
{

}

const std::string MapIndexOutOfBounds::GetErrorMessage() const
{
    std::stringstream ss;
    ss << "IndexOutOfBounds: Target(" << _x << ", " << _y << ")";
    ss << " vs " << "MapSize(" << _map.GetWidht() << ", " << _map.GetHeight() << ")\n";
    return ss.str();
}


MapInvalidUnitPosition::MapInvalidUnitPosition(const Map& map, int x, int y) : _x{x}, _y{y}, MapException(map)
{

}

const std::string MapInvalidUnitPosition::GetErrorMessage() const
{
    std::stringstream ss;
    ss << "MapInvalidUnitPosition: There was a unit already in Pos(" << _x << ", " << _y << ")\n";
    return ss.str();
}
