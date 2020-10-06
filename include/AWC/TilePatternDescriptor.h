#include <Utils/Vector2.h>
#include <AWC/AWCfwd.h>

#include <vector>
#include <memory>


class TilePatternDescriptor
{
public:
    std::shared_ptr<TilePattern> CreateTilePattern(Vector2 origin);
    std::shared_ptr<TilePattern> CreateTilePatternWithDestination(Vector2 origin, Vector2 destination);

private:
    TilePatternDescriptor(int minDist, int maxDist, const TilePatternDescriptorType& type);
    std::vector<std::weak_ptr<TileNode>> DiscoverNeighbours(const Map& map, TileGraph& mg, int x, int y, const std::vector<Vector2>& directions);

    int minDist_;
    int maxDist_;
    const TilePatternDescriptorType& type_;
};