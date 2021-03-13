

#include <DB/AWCSer.h>
#include <DB/JsonUtils.h>

#include <AWC/TileType.h>

#include <iostream>

TileType AWCSer::LoadTileType(Json data)
{
    uint id = JsonUtils::GetValue(data, "id", -1);
    std::string name = JsonUtils::GetValue<std::string>(data, "name", "UNKNOWN");

    return TileType{id, name};
}

TilePtr AWCSer::LoadTile(Json data, Repository<TileType>& tileTypeRepo)
{
    uint tileTypeId = JsonUtils::GetValue<uint>(data, "tileTypeId");
    auto& tileType = tileTypeRepo.GetById(tileTypeId);
    return tileType.CreateTile();
}

Vector2 AWCSer::LoadDirection(Json data)
{
    int x = JsonUtils::GetValue<int>(data, "x");
    int y = JsonUtils::GetValue<int>(data, "y");

    return Vector2{x, y};
}