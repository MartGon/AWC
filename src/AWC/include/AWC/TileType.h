#pragma once

#include <memory>
#include <string>

class Tile;

class TileType
{
public:
    TileType(unsigned int id, const std::string& name);

    std::shared_ptr<Tile> CreateTile() const;

    unsigned int GetId() const;
    std::string GetName() const;

private:

    unsigned int id_;
    std::string name_;

    // Static stuff
    // int defense
    // int income
};