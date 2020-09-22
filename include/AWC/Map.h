#pragma once

#include <vector>
#include <exception>
#include <string>

#include <AWC/Unit.h>
#include <AWC/Tile.h>

class Map
{
public:
    Map(int x, int y);

    int GetWidht();
    int GetHeight();

    void AddUnit(int x, int y, Unit* unit);
    Unit* GetUnit(int x, int y);

    void SetTile(int x, int y, Tile* tile);
    Tile* GetTile(int x, int y);

private:

    bool IsPositionFree(int x, int y);
    bool IsPositionValid(int x, int y);

    int _x;
    int _y;

    std::vector<std::vector<Unit*>> _units;
    std::vector<std::vector<Tile*>> _tiles; 
};

class MapIndexOutOfBounds : public std::exception
{
public:
    MapIndexOutOfBounds(const std::string& msg);
    virtual char const* what();
private:
    std::string _msg;
};

class MapInvalidUnitPosition : public std::exception
{
public:
    MapInvalidUnitPosition(const std::string& msg);
    virtual char const* what();
private:
    std::string _msg;
};