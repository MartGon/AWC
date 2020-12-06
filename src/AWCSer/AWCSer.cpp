
#include <AWCSer/AWCSer.h>

#include <AWCSer/JsonUtils.h>

#include <AWC/TileType.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/TilePatternDescComp.h>
#include <AWC/TilePatternComp.h>

#include <iostream>

TileType AWCSer::LoadTileType(Json data)
{
    uint id = JsonUtils::GetValue(data, "id", -1);
    std::string name = JsonUtils::GetValue<std::string>(data, "name", "UNKNOWN");

    return TileType{id, name};
}

TilePtr AWCSer::LoadTile(Json data, Repository<TileType> tileTypeRepo)
{
    uint tileTypeId = JsonUtils::GetValue<uint>(data, "tileTypeId");
    auto& tileType = tileTypeRepo.GetById(tileTypeId);
    return tileType.CreateTile();
}

TilePatternDescIPtr AWCSer::LoadTilePatternDescI(Json data, Repository<TilePatternDescIPtr> repo)
{
    TilePatternDescIPtr tpdip;
    auto type = JsonUtils::GetValue<TilePatternDescType>(data, "type", TilePatternDescType::BASE);
    switch (type)
    {
    case TilePatternDescType::BASE:
        tpdip = LoadTilePatternDesc(data);
    case TilePatternDescType::COMP:
        tpdip = LoadTilePatternDescComp(data, repo);
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

TilePatternDescIPtr AWCSer::LoadTilePatternDescComp(Json data, Repository<TilePatternDescIPtr> repo)
{
    auto subType = JsonUtils::GetValue<TilePatternDescCompType>(data, "subType");
    auto tpdAId = JsonUtils::GetValue<uint>(data, "tpdA");
    auto tpdBId = JsonUtils::GetValue<uint>(data, "tpdB");

    auto tpdA = repo.GetById(tpdAId);
    auto tpdB = repo.GetById(tpdBId);

    TilePatternDescIPtr tpdip;
    switch (subType)
    {
    case TilePatternDescCompType::UNION:
        tpdip = TilePatternDescIPtr{ new TilePatternDescComp<TilePatternUnion>{tpdA, tpdB}};
        break;
    case TilePatternDescCompType::DIFF:
        tpdip = TilePatternDescIPtr{ new TilePatternDescComp<TilePatternDiff>{tpdA, tpdB}};
        break;
    case TilePatternDescCompType::INTERSECT:
        tpdip = TilePatternDescIPtr{ new TilePatternDescComp<TilePatternIntersect>{tpdA, tpdB}};
        break;
    default:
        break;
    }

    return tpdip;
}

Vector2 AWCSer::LoadDirection(Json data)
{
    int x = JsonUtils::GetValue<int>(data, "x");
    int y = JsonUtils::GetValue<int>(data, "y");

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