#include <AWC/AWCusing.h>
#include <Utils/Vector2.h>

namespace Pathfinding
{
    struct Params
    {
        Params(const Map& map, TilePatternDesc& tpd, const TilePatternConstraints& constraints, std::optional<Vector2> dest) :
            map{map}, tpd{tpd}, constraints{constraints}, dest{dest} {}

        const Map& map;
        TilePatternDesc& tpd;
        const TilePatternConstraints& constraints;
        std::optional<Vector2> dest;
    };

    TileGraph Dijkstra(Vector2 origin, Params params);

    Directions GetDiscoverDirections(TileNodePtr tileNode, Params params);
    Vector2 GetMovementToOrigin(TileNodePtr tileNode);
    Directions GetValidDirections(TileNodePtr tileNode, Directions directions, const Map& map);
    unsigned int GetTileCost(const Map& map, const TilePatternConstraints& tpc, Vector2 pos);

}