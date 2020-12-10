#include <AWCSer/TilePatternDescSer.h>
#include <AWCSer/JsonUtils.h>
#include <AWCSer/AWCSer.h>

#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/TilePattern/TilePatternDescComp.h>
#include <AWC/TilePattern/TilePatternComp.h>

TilePatternDescIPtr TilePatternDescSer::LoadTilePatternDescI(Json data, Repository<TilePatternDescIPtr> repo)
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

TilePatternDescPtr TilePatternDescSer::LoadTilePatternDesc(Json data)
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
            Vector2 indexDir = AWCSer::LoadDirection(indexObj);
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

TilePatternDescIPtr TilePatternDescSer::LoadTilePatternDescComp(Json data, Repository<TilePatternDescIPtr> repo)
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

std::vector<Vector2> TilePatternDescSer::LoadDirections(Json data)
{
    Directions directions;
    for(auto entry : data)
    {
        Vector2 dir = AWCSer::LoadDirection(entry);
        directions.push_back(dir);
    }

    return directions;
}