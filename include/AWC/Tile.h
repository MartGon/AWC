#pragma once

#include <string>

class TileType;

class Tile
{
friend class TileType;

public:

    std::string GetName();

private:
    Tile(TileType& tileType);

    TileType& tileType_;

    // State information
    // Team CapturedBy;
};