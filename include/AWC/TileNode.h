#pragma once

#include <Utils/Vector2.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class TileGraph;
class TileNode;

using TileNodePtr = std::weak_ptr<TileNode>;

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

// Exceptions

class TileNodeException : public std::exception
{
public:
    TileNodeException(const std::string& msg, Vector2 pos);
    const char* what() const noexcept;

    Vector2 pos;
private:
    std::string msg_;
};

class TileNodeNoExistingNeighbour : public TileNodeException
{
public:
    TileNodeNoExistingNeighbour(Vector2 pos) : TileNodeException(std::string("Neighbour at pos ") + pos + " did not exist", pos) {};
};

class MapNodeAlreadyExistingNeigbour : public TileNodeException
{
public:
    MapNodeAlreadyExistingNeigbour(Vector2 pos) : TileNodeException(std::string("Neighbour at pos ") + pos + " already exists", pos) {};
};
