#pragma once

#include <Utils.h>

#include <AWC/AWCfwd.h>
#include <AWC/Area/Node.h>
#include <AWC/Directions.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <string>

// MapGraph

class Graph
{
using NodePtr = std::weak_ptr<Node>;
public:
    Graph(){}
    Graph(Graph& tg);
    Graph(Graph&& mg);

    NodePtr CreateNode(const Vector2 pos, const unsigned int cost);
    NodePtr CreateNode(const Vector2 pos, const unsigned int cost, const Vector2 nei);

    std::vector<Vector2> GetNodesPos() const;
    NodePtr GetNode(Vector2 pos) const;
    bool NodeExists(Vector2 pos) const;
    
    void SetNeighbour(Vector2 a, Vector2 b);

    std::vector<NodePtr> DiscoverNeighbours(Vector2 pos, const Directions& directions);    

#ifdef _DEBUG
    bool moved = false;
#endif

private:
    std::unordered_map<Vector2, std::shared_ptr<Node>> nodes_;
};
