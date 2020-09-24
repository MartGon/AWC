#pragma once

#include <memory>
#include <string>

class Tile;

class TileType
{
public:
    TileType(const std::string& name);

    std::shared_ptr<Tile> CreateTile();

    std::string GetName() const;

private:

    std::string name_;

    // Static stuff
    // int defense
    // int income
};