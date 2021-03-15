#include <AWC/AWCusing.h>
#include <Utils.h>

namespace Pathfinding
{
    struct Params
    {
        Params(const Map& map, const AreaDesc& tpd, const AreaConstraints& constraints, std::optional<Vector2> dest) :
            map{map}, tpd{tpd}, constraints{constraints}, dest{dest} {}

        const Map& map;
        const AreaDesc& tpd;
        const AreaConstraints& constraints;
        std::optional<Vector2> dest;
    };

    Graph Dijkstra(Vector2 origin, Params params);

    Directions GetDiscoverDirections(NodePtr tileNode, Params params);
    Vector2 GetMovementToOrigin(NodePtr tileNode);
    Directions GetValidDirections(NodePtr tileNode, Directions directions, const Map& map);
    unsigned int GetTileCost(const Map& map, const AreaConstraints& tpc, Vector2 pos);

}