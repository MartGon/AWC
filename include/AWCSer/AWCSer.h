#include <AWC/AWCusing.h>
#include <AWCSer/Repository.h>

#include <json.hpp>

using Json = nlohmann::json;

namespace AWCSer
{
    TileType LoadTileType(Json data);
    TilePtr LoadTile(Json data, Repository<TileType> tileTypeRepo);
}