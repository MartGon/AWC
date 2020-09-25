#pragma once

#include <memory>
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

    void AddUnit(int x, int y, std::shared_ptr<Unit> unit);
    const std::shared_ptr<Unit> GetUnit(int x, int y) const;
    void RemoveUnit(int x, int y);

    void SetTile(int x, int y, std::shared_ptr<Tile> tile);
    const std::shared_ptr<Tile> GetTile(int x, int y) const;

    bool IsPositionFree(int x, int y) const;
    bool IsPositionValid(int x, int y) const;

private:

    int x_;
    int y_;

    std::vector<std::vector<std::shared_ptr<Unit>>> units_;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles_; 
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