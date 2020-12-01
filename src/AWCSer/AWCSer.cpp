#include <AWCSer/AWCSer.h>

#include <AWCSer/AWCSer.h>
#include <AWCSer/JsonUtils.h>

#include <AWC/TileType.h>

TileType AWCSer::LoadTileType(Json data)
{
    uint id = JsonUtils::GetValue(data, "id", -1);
    std::string name = JsonUtils::GetValue<std::string>(data, "name", "UNKNOWN");

    return TileType{id, name};
}