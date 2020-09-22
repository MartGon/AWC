#include <AWC/Map.h>

// Public interface

Map::Map(int x, int y) : _x{x}, _y{y}
{
    _units.reserve(x);
    for(int i = 0; i < x; i++)
        _units[i] = std::vector<Unit*>{(long unsigned int)y};
}

int Map::GetWidht()
{
    return _x;
}

int Map::GetHeight()
{
    return _y;
}

void Map::AddUnit(int x, int y, Unit* unit)
{
    if(IsPositionFree(x, y))
        _units[x][y] = unit;
    else
        throw MapInvalidUnitPosition("");
}

Unit* Map::GetUnit(int x, int y)
{
    Unit* unit = nullptr;
    if(IsPositionValid(x, y))
        unit = _units[x][y];
    else
        throw MapIndexOutOfBounds("");

    return unit;
}

// Private methods

bool Map::IsPositionFree(int x, int y)
{
    return GetUnit(x, y) == nullptr;
}

bool Map::IsPositionValid(int x, int y)
{
    return x >= 0 && x < _x && y >= 0 && y < _y;
}

// Exceptions

MapIndexOutOfBounds::MapIndexOutOfBounds(const std::string& msg) : _msg{msg}
{

}

const char* MapIndexOutOfBounds::what()
{
    return _msg.c_str();
}


MapInvalidUnitPosition::MapInvalidUnitPosition(const std::string& msg) : _msg{msg}
{

}

const char* MapInvalidUnitPosition::what()
{
    return _msg.c_str();
}
