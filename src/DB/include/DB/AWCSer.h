#pragma once
#include <AWC.h>
#include <DB/Repository.h>

#include <json.hpp>

using Json = nlohmann::json;

namespace AWCSer
{
    TileType LoadTileType(Json data);
    TilePtr LoadTile(Json data, Repository<TileType> tileTypeRepo);

    Vector2 LoadDirection(Json data);
}