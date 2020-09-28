#include <AWC/TilePattern.h>

#include <vector>

#include <AWC/Map.h>

ManhatanPattern::ManhatanPattern(int minDist, int maxDist) : minDist_{minDist}, maxDist_{maxDist_}
{

}

bool ManhatanPattern::IsTileInRange(const Map& map, int originX, int originY, int destX, int destY)
{
    std::vector<Vector2> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    bool inRange = 0;
    Node firstNode{map, {originX, originY}, 0};

    firstNode.DiscoverNeighbours(directions);

    return false;
}

Node::Node(const Map& map, Vector2 pos, int cost) : map_{map}, pos_{pos}, cost_{cost}
{

}

Node::Node(const Map& map, int x, int y, int cost) : Node{map, Vector2{x, y}, cost} 
{

}

void Node::DiscoverNeighbours(const std::vector<Vector2>& directions)
{
    for(const auto& dir : directions)
    {
        Vector2 tilePos = pos_ + dir;
        if(map_.IsPositionValid(tilePos.x, tilePos.y))
        {
            Node* neighbour = new Node(map_, tilePos, 1);
            this->AddNeighbour(neighbour);
        }
    }
}