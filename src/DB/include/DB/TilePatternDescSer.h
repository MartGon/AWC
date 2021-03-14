#pragma once

#include <AWC.h>
#include <DB/Repository.h>

#include <json.hpp>

using Json = nlohmann::json;

namespace TilePatternDescSer
{

    enum class TilePatternDescType
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

    enum TilePatternDescCompType
    {
        UNION,
        DIFF,
        INTERSECT
    };

    enum TilePatternDescDecType
    {
        STATIC_RANGE,
        STATIC_COST
    };

    TilePatternDescIPtr LoadTilePatternDescI(Json data, Repository<TilePatternDescIPtr>& repo);
    TilePatternDescPtr LoadTilePatternDesc(Json data);
    TilePatternDescIPtr LoadTilePatternDescComp(Json data, Repository<TilePatternDescIPtr>& repo);
    std::vector<Vector2> LoadDirections(Json data);

}