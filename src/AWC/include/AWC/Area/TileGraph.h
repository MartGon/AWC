#pragma once

#include <Utils.h>

#include <AWC/AWCfwd.h>
#include <AWC/Area/TileNode.h>
#include <AWC/Directions.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <string>

// MapGraph

class TileGraph
{
using TileNodePtr = std::weak_ptr<TileNode>;
public:
    TileGraph(){}
    TileGraph(TileGraph& tg);
    TileGraph(TileGraph&& mg);

    TileNodePtr CreateNode(const Vector2 pos, const unsigned int cost);
    TileNodePtr CreateNode(const Vector2 pos, const unsigned int cost, const Vector2 nei);

    std::vector<Vector2> GetNodesPos() const;
    TileNodePtr GetNode(Vector2 pos) const;
    bool NodeExists(Vector2 pos) const;
    
    void SetNeighbour(Vector2 a, Vector2 b);

    std::vector<TileNodePtr> DiscoverNeighbours(Vector2 pos, const Directions& directions);    

#ifdef _DEBUG
    bool moved = false;
#endif

private:
    std::unordered_map<Vector2, std::shared_ptr<TileNode>> nodes_;
};
