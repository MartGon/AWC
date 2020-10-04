#include <Utils/Vector2.h>

#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class MapGraph;

class MapNode
{

friend class MapGraph;

public:
    std::weak_ptr<MapNode> GetNeighbour(Vector2 pos);
    std::vector<std::weak_ptr<MapNode>> GetNeighbours();

    bool NeighbourExists(Vector2 pos);

    const Vector2 pos;
    int cost;
    int& steps;
    bool visited;
private:
    void AddNeigbour(Vector2 pos, std::weak_ptr<MapNode> neighbour);
    MapNode(const Vector2 pos, const int cost);

    std::unordered_map<Vector2, std::weak_ptr<MapNode>> neighbours_;
};

// Exceptions

class MapNodeException : public std::exception
{
public:
    MapNodeException(const std::string& msg, Vector2 pos);
    const char* what() const noexcept;

    Vector2 pos;
private:
    std::string msg_;
};

class MapNodeNoExistingNeighbour : public MapNodeException
{
public:
    MapNodeNoExistingNeighbour(Vector2 pos) : MapNodeException(std::string("Neighbour at pos ") + pos + " did not exist", pos) {};
};

class MapNodeAlreadyExistingNeigbour : public MapNodeException
{
public:
    MapNodeAlreadyExistingNeigbour(Vector2 pos) : MapNodeException(std::string("Neighbour at pos ") + pos + " already exists", pos) {};
};

// MapGraph

class MapGraph
{
public:

    std::weak_ptr<MapNode> CreateNode(const Vector2 pos, const int cost);
    std::weak_ptr<MapNode> GetNode(Vector2 pos);
    bool NodeExists(Vector2 pos);
    
    void SetNeighbour(Vector2 a, Vector2 b);

private:
    std::unordered_map<Vector2, std::shared_ptr<MapNode>> nodes_;
};

// Exceptions

class MapGraphException : public std::exception
{
public:
    MapGraphException(const std::string& msg, Vector2 pos);
    const char* what() const noexcept override;

    Vector2 pos;
private:
    std::string msg_;
};

class MapGraphNoExistingNode : public MapGraphException
{
public:
    MapGraphNoExistingNode(Vector2 pos) : MapGraphException(std::string("Node at pos ") + pos + std::string(" didn't exist"), pos) {};
};

class MapGraphAlreadyExistingNode : public MapGraphException
{
public:
    MapGraphAlreadyExistingNode(Vector2 pos) : MapGraphException(std::string("Node at pos ") + pos + std::string(" already existed"), pos) {};
};