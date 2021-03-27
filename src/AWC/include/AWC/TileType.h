#pragma once

#include <memory>
#include <string>

class Tile;

class TileType
{
public:
    TileType(unsigned int id, std::string name);

    std::shared_ptr<Tile> CreateTile();

    unsigned int GetId() const;
    std::string GetName() const;

private:

    uint lastInstanceId_ = 0;
    unsigned int id_;
    std::string name_;

    // Static stuff
    // int defense
    // int income
};