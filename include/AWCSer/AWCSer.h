#include <AWC/AWCusing.h>
#include <json.hpp>

using Json = nlohmann::json;

namespace AWCSer
{
    TileType LoadTileType(Json data);
}