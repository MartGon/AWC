#pragma once

#include <AWC/AWCusing.h>
#include <Utils.h>


#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class Graph;
class Node;

class Node
{

friend class Graph;

public:
    NodePtr GetNeighbour(Vector2 pos);
    std::vector<NodePtr> GetNeighbours();

    bool NeighbourExists(Vector2 pos);

    NodePtr GetNeighbourBySortCriteria(std::function<bool(NodePtr a, NodePtr b)> sortCriteria);

    const Vector2 pos;
    unsigned int cost;
private:
    void AddNeigbour(Vector2 pos, NodePtr neighbour);
    Node(const Vector2 pos, const unsigned int cost);

    std::unordered_map<Vector2, NodePtr> neighbours_;
};
