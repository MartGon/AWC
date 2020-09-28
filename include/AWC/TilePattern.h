#include <Utils/Vector2.h>

#include <vector>

class Map;
class Tile;

class TilePattern
{
public:
    virtual ~TilePattern() {};
    virtual bool IsTileInRange(const Map& map, int originX, int originY, int destX, int destY) = 0;
};

class ManhatanPattern : public TilePattern
{
public:
    ManhatanPattern(int minDist, int maxDist);

    bool IsTileInRange(const Map& map, int originX, int originY, int destX, int destY) override;

private:
    int minDist_;
    int maxDist_;
};

class Node
{
public:
    Node(const Map& map, int x, int y, int cost);
    Node(const Map& map, Vector2 pos, int cost);

    void DiscoverNeighbours(const std::vector<Vector2>& directions);

    void AddNeighbour(Node* neighbour)
    {
        neighbours.push_back(neighbour);
    };

private:
    Vector2 pos_;
    int cost_;
    const Map& map_;
    std::vector<Node*> neighbours;
};