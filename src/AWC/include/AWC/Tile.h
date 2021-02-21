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
    Tile(const TileType& tileType);

    const TileType& tileType_;

    // State information
    // Team CapturedBy;
};