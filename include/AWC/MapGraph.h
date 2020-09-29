#include <Utils/Vector2.h>

#include <memory>
#include <unordered_map>
#include <vector>

class MapGraph;

class MapNode
{

friend class MapGraph;

public:
    void AddNeigbour(Vector2 pos, std::weak_ptr<MapNode> neighbour);
    std::weak_ptr<MapNode> GetNeighbour(Vector2 pos);
    std::vector<std::weak_ptr<MapNode>> GetNeighbours();

    const Vector2 pos;
    const int cost;
private:
    MapNode(const Vector2 pos, const int cost);

    std::unordered_map<Vector2, std::weak_ptr<MapNode>> neighbours_;
};

class MapGraph
{
public:

    std::weak_ptr<MapNode> CreateNode(const Vector2 pos, const int cost);
    std::weak_ptr<MapNode> GetNode(Vector2 pos);
    bool NodeExists(Vector2 pos);

private:
    std::unordered_map<Vector2, std::shared_ptr<MapNode>> nodes_;
};