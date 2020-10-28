#pragma once

#include <AWC/AWCusing.h>
#include <Utils/Vector2.h>


#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class TileGraph;
class TileNode;

class TileNode
{

friend class TileGraph;

public:
    TileNodePtr GetNeighbour(Vector2 pos);
    std::vector<TileNodePtr> GetNeighbours();

    bool NeighbourExists(Vector2 pos);

    TileNodePtr GetNeighbourBySortCriteria(std::function<bool(TileNodePtr a, TileNodePtr b)> sortCriteria);

    const Vector2 pos;
    unsigned int cost;
private:
    void AddNeigbour(Vector2 pos, TileNodePtr neighbour);
    TileNode(const Vector2 pos, const unsigned int cost);

    std::unordered_map<Vector2, TileNodePtr> neighbours_;
};
