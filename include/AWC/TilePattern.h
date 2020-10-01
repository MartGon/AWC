#include <Utils/Vector2.h>

#include <vector>
#include <memory>

class Map;
class Tile;
class MapNode;
class MapGraph;

class TilePattern
{
public:
    virtual ~TilePattern() {};
    virtual bool IsTileInRange(const Map& map, int originX, int originY, int destX, int destY) = 0;
};

class ManhattanPattern : public TilePattern
{
public:
    ManhattanPattern(int minDist, int maxDist);

    bool IsTileInRange(const Map& map, int originX, int originY, int destX, int destY) override;

private:

    std::vector<std::weak_ptr<MapNode>> DiscoverNeighbours(const Map& map, MapGraph& mg, int x, int y, const std::vector<Vector2>& directions);

    int minDist_;
    int maxDist_;
};