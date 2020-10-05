#pragma once

// Own headers
#include <Utils/Vector2.h>

// STL
#include <vector>
#include <memory>


class TilePatternDescriptorType
{
friend class TilePatternDescriptor;

public:
    CreateTilePatternDescriptor(int minDist, int maxDist);

    std::vector<Vector2> GetDirections();
    std::vector<Vector2> GetExclusiveDirections(Vector2 dir);
    std::vector<Vector2> GetLockedDirections(Vector2 dir);

private:
    std::vector<Vector2> directions_;
    std::unordered_map<Vector2, std::vector<Vector2>> exclusiveDirectionsMap_;
};