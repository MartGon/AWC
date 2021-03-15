#pragma once

#include <AWC.h>
#include <DB/Repository.h>

#include <json.hpp>

using Json = nlohmann::json;

namespace AreaDescSer
{

    enum class AreaDescType
    {
        BASE,
        COMP,
        DECORATOR
    };

    enum class DirectionsTableType
    {
        LOCKED,
        EXCLUSIVE
    };

    enum AreaDescCompType
    {
        UNION,
        DIFF,
        INTERSECT
    };

    enum AreaDescDecType
    {
        STATIC_RANGE,
        STATIC_COST
    };

    AreaDescIPtr LoadAreaDescI(Json data, Repository<AreaDescIPtr>& repo);
    AreaDescPtr LoadAreaDesc(Json data);
    AreaDescIPtr LoadAreaDescComp(Json data, Repository<AreaDescIPtr>& repo);
    std::vector<Vector2> LoadDirections(Json data);

}