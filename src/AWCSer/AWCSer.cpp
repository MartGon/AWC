#include <AWCSer/AWCSer.h>

#include <AWCSer/JsonUtils.h>

#include <AWC/TileType.h>
#include <AWC/TilePatternDesc.h>

#include <iostream>

TileType AWCSer::LoadTileType(Json data)
{
    uint id = JsonUtils::GetValue(data, "id", -1);
    std::string name = JsonUtils::GetValue<std::string>(data, "name", "UNKNOWN");

    return TileType{id, name};
}

TilePtr AWCSer::LoadTile(Json data, Repository<TileType> tileTypeRepo)
{
    uint tileTypeId = JsonUtils::GetValue(data, "tileTypeId", -1);
    auto& tileType = tileTypeRepo.GetById(tileTypeId);
    return tileType.CreateTile();
}

TilePatternDescIPtr AWCSer::LoadTilePatternDescI(Json data)
{
    TilePatternDescIPtr tpdip;
    auto type = JsonUtils::GetValue<TilePatternDescType>(data, "type", TilePatternDescType::BASE);
    switch (type)
    {
    case TilePatternDescType::BASE:
        tpdip = LoadTilePatternDesc(data);
    default:
        break;
    }

    return tpdip;
}

TilePatternDescPtr AWCSer::LoadTilePatternDesc(Json data)
{
    TilePatternDescPtr tpd;

    auto originDirectionsObj = JsonUtils::GetValue<Json::array_t>(data, "originDirections");
    Directions originDirections = LoadDirections(originDirectionsObj);

    auto directionsTableObj = JsonUtils::GetValue<Json>(data, "directionsTable", Json{});
    if(directionsTableObj.is_object())
    {
        auto tableObj = JsonUtils::GetValue<Json::array_t>(directionsTableObj, "table");

        std::cout << directionsTableObj << "\n";

        DirectionsTable directionsTable;
        for(auto entry : tableObj)
        {
            auto indexObj = JsonUtils::GetValue<Json>(entry, "indexDir");
            Vector2 indexDir = LoadDirection(indexObj);
            auto directionsObj = JsonUtils::GetValue<Json::array_t>(entry, "directions");
            Directions dirs = LoadDirections(directionsObj);
            directionsTable.insert({indexDir, dirs});
        }

        auto tableType = JsonUtils::GetValue<DirectionsTableType>(directionsTableObj, "type", DirectionsTableType::LOCKED);
        if(tableType == DirectionsTableType::LOCKED)
            tpd = TilePatternDesc::CreateByLocked(originDirections, directionsTable);
        else if(tableType == DirectionsTableType::EXCLUSIVE)
            tpd = TilePatternDesc::CreateByExclusive(originDirections, directionsTable);
    }
    else
        tpd = TilePatternDesc::Create(originDirections);

    return tpd;
}

Vector2 AWCSer::LoadDirection(Json data)
{
    int x = JsonUtils::GetValue(data, "x", 0);
    int y = JsonUtils::GetValue(data, "y", 0);

    return Vector2{x, y};
}

std::vector<Vector2> AWCSer::LoadDirections(Json data)
{
    Directions directions;
    for(auto entry : data)
    {
        Vector2 dir = LoadDirection(entry);
        directions.push_back(dir);
    }

    return directions;
}