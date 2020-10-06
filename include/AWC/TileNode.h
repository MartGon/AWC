#pragma once

#include <Utils/Vector2.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class TileGraph;

class TileNode
{

friend class TileGraph;

public:
    std::weak_ptr<TileNode> GetNeighbour(Vector2 pos);
    std::vector<std::weak_ptr<TileNode>> GetNeighbours();

    bool NeighbourExists(Vector2 pos);

    const Vector2 pos;
    int cost;
    int& steps;
    bool visited;
private:
    void AddNeigbour(Vector2 pos, std::weak_ptr<TileNode> neighbour);
    TileNode(const Vector2 pos, const int cost);

    std::unordered_map<Vector2, std::weak_ptr<TileNode>> neighbours_;
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
