#pragma once

#include <Utils/Vector2.h>
#include <AWC/AWCfwd.h>
#include <AWC/TileNode.h>
#include <AWC/Directions.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <string>

using TileNodePtr = std::weak_ptr<TileNode>;

// MapGraph

class TileGraph
{
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

    std::vector<TileNodePtr> DiscoverNeighbours(Vector2 pos, const Directions& directions, const TilePatternConstraints& tpc);    

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