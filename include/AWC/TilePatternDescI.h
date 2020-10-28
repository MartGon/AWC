#pragma once
#include <AWC/AWCusing.h>
#include <Utils/Vector2.h>

#include <memory>
#include <optional>

class TilePatternDescI
{
public:
    virtual ~TilePatternDescI() {}

    TilePatternIPtr CalculateTilePattern(Vector2 origin, const Map& map, const TilePatternConstraints& constraints);
    TilePatternIPtr CalculateTilePattern(Vector2 origin, Vector2 destination, const Map& map, const TilePatternConstraints& constraints);

private:
    virtual TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) = 0;
};