#include <AWC/Map.h>

#include <sstream>

// Public interface

Map::Map(int x, int y) : _x{x}, _y{y}
{
    _units.resize(x);
    for(int i = 0; i < x; i++)
        _units[i] = std::vector<Unit*>{(long unsigned int)y};

    
    _tiles.resize(x);
    for(int i = 0; i < x; i++)
        _tiles[i] = std::vector<Tile*>{(long unsigned int)y};
    
}

int Map::GetWidht() const
{
    return _x;
}

int Map::GetHeight() const
{
    return _y;
}

void Map::AddUnit(int x, int y, Unit* unit)
{
    if(IsPositionFree(x, y))
        _units[x][y] = unit;
    else
        throw MapInvalidUnitPosition(*this, x, y);
}

Unit* Map::GetUnit(int x, int y) const
{
    Unit* unit = nullptr;
    if(IsPositionValid(x, y))
        unit = _units[x][y];
    else
        throw MapIndexOutOfBounds(*this, x, y);

    return unit;
}

void Map::SetTile(int x, int y, Tile* tile)
{
    throw MapIndexOutOfBounds(*this, x, y);
}

Tile* Map::GetTile(int x, int y)
{
    throw MapIndexOutOfBounds(*this, x, y);
}

// Private methods

bool Map::IsPositionFree(int x, int y) const
{
    return GetUnit(x, y) == nullptr;
}

bool Map::IsPositionValid(int x, int y) const
{
    return x >= 0 && x < _x && y >= 0 && y < _y;
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
