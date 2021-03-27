#pragma once

#include <string>

class TileType;

class Tile
{
friend class TileType;

public:

    unsigned int GetId() const;
    std::string GetName() const;

private:
    Tile(const TileType& tileType, unsigned int id);

    const TileType& tileType_;

    unsigned int id_;
    // State information
    // Team CapturedBy;
};