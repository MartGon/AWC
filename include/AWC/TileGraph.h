#pragma once

#include <Utils/Vector2.h>
#include <AWC/TileNode.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>


// MapGraph

class TileGraph
{
public:
    TileGraph(){}
    TileGraph(TileGraph&& mg);
    std::weak_ptr<TileNode> CreateNode(const Vector2 pos, const int cost);
    std::weak_ptr<TileNode> CreateNode(const Vector2 pos, const int cost, const Vector2 nei);
    std::weak_ptr<TileNode> GetNode(Vector2 pos);
    bool NodeExists(Vector2 pos);
    
    void SetNeighbour(Vector2 a, Vector2 b);

#ifdef _DEBUG
    bool moved = false;
#endif

private:
    std::unordered_map<Vector2, std::shared_ptr<TileNode>> nodes_;
};

// Exceptions

class TileGraphException : public std::exception
{
public:
    TileGraphException(const std::string& msg, Vector2 pos);
    const char* what() const noexcept override;

    Vector2 pos;
private:
    std::string msg_;
};

class TileGraphNoExistingNode : public TileGraphException
{
public:
    TileGraphNoExistingNode(Vector2 pos) : TileGraphException(std::string("Node at pos ") + pos + std::string(" didn't exist"), pos) {};
};

class TileGraphAlreadyExistingNode : public TileGraphException
{
public:
    TileGraphAlreadyExistingNode(Vector2 pos) : TileGraphException(std::string("Node at pos ") + pos + std::string(" already existed"), pos) {};
};