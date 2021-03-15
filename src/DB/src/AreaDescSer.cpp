#include <DB/AreaDescSer.h>
#include <DB/JsonUtils.h>
#include <DB/AWCSer.h>

#include <AWC.h>

AreaDescIPtr AreaDescSer::LoadAreaDescI(Json data, Repository<AreaDescIPtr>& repo)
{
    AreaDescIPtr tpdip;
    auto type = JsonUtils::GetValue<AreaDescType>(data, "type", AreaDescType::BASE);
    switch (type)
    {
    case AreaDescType::BASE:
        tpdip = LoadAreaDesc(data);
    case AreaDescType::COMP:
        tpdip = LoadAreaDescComp(data, repo);
    default:
        break;
    }

    return tpdip;
}

AreaDescPtr AreaDescSer::LoadAreaDesc(Json data)
{
    AreaDescPtr tpd;

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
            tpd = AreaDesc::CreateByLocked(originDirections, directionsTable);
        else if(tableType == DirectionsTableType::EXCLUSIVE)
            tpd = AreaDesc::CreateByExclusive(originDirections, directionsTable);
    }
    else
        tpd = AreaDesc::Create(originDirections);

    return tpd;
}

AreaDescIPtr AreaDescSer::LoadAreaDescComp(Json data, Repository<AreaDescIPtr>& repo)
{
    auto subType = JsonUtils::GetValue<AreaDescCompType>(data, "subType");
    auto tpdAId = JsonUtils::GetValue<uint>(data, "tpdA");
    auto tpdBId = JsonUtils::GetValue<uint>(data, "tpdB");

    auto tpdA = repo.GetById(tpdAId);
    auto tpdB = repo.GetById(tpdBId);

    AreaDescIPtr tpdip;
    switch (subType)
    {
    case AreaDescCompType::UNION:
        tpdip = AreaDescIPtr{ new AreaDescComp<AreaUnion>{tpdA, tpdB}};
        break;
    case AreaDescCompType::DIFF:
        tpdip = AreaDescIPtr{ new AreaDescComp<AreaDiff>{tpdA, tpdB}};
        break;
    case AreaDescCompType::INTERSECT:
        tpdip = AreaDescIPtr{ new AreaDescComp<AreaIntersect>{tpdA, tpdB}};
        break;
    default:
        break;
    }

    return tpdip;
}

std::vector<Vector2> AreaDescSer::LoadDirections(Json data)
{
    Directions directions;
    for(auto entry : data)
    {
        Vector2 dir = AWCSer::LoadDirection(entry);
        directions.push_back(dir);
    }

    return directions;
}