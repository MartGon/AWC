#include <AWC/AWCusing.h>
#include <AWCSer/Repository.h>

#include <json.hpp>

using Json = nlohmann::json;

namespace AWCSer
{
    TileType LoadTileType(Json data);
    TilePtr LoadTile(Json data, Repository<TileType> tileTypeRepo);

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

    TilePatternDescIPtr LoadTilePatternDescI(Json data, Repository<TilePatternDescIPtr> repo = Repository<TilePatternDescIPtr>{});
    TilePatternDescPtr LoadTilePatternDesc(Json data);
    TilePatternDescIPtr LoadTilePatternDescComp(Json data, Repository<TilePatternDescIPtr> repo);
    Vector2 LoadDirection(Json data);
    std::vector<Vector2> LoadDirections(Json data);
}