#pragma once

#include <string>

class Tile;

class TileType
{
public:
    TileType(const std::string& name);

    Tile* CreateTile();

    std::string GetName() const;

private:

    std::string _name;

    // Static stuff
    // int defense
    // int income
};