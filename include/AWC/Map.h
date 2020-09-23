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

    int GetWidht() const;
    int GetHeight() const;

    void AddUnit(int x, int y, Unit* unit);
    Unit* GetUnit(int x, int y) const;

    void SetTile(int x, int y, Tile* tile);
    Tile* GetTile(int x, int y);

private:

    bool IsPositionFree(int x, int y) const;
    bool IsPositionValid(int x, int y) const;

    int _x;
    int _y;

    std::vector<std::vector<Unit*>> _units;
    std::vector<std::vector<Tile*>> _tiles; 
};

class MapException : public std::exception
{
public:
    MapException(const Map& map);
    virtual char const* what() const noexcept override;
protected:
    const Map& _map;
private:
    virtual const std::string GetErrorMessage() const = 0;
};

class MapIndexOutOfBounds : public MapException
{
public:
    MapIndexOutOfBounds(const Map&, int x, int y);
private:
    int _x;
    int _y;
    const std::string GetErrorMessage() const override;
};

class MapInvalidUnitPosition : public MapException
{
public:
    MapInvalidUnitPosition(const Map& map, int x, int y);
private:
    int _x;
    int _y;
    const std::string GetErrorMessage() const override;
};