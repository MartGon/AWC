#pragma once
#include <AWC/AWCusing.h>
#include <Utils/Vector2.h>

#include <memory>
#include <optional>

class TilePatternDescI
{
public:
    virtual ~TilePatternDescI() {}

    TilePatternIPtr CalculateTilePattern(const Map& map, Vector2 origin, const TilePatternConstraints& constraints);
    TilePatternIPtr CalculateTilePattern(const Map& map, Vector2 origin, Vector2 destination, const TilePatternConstraints& constraints);

private:
    virtual TilePatternIPtr DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const TilePatternConstraints& constraints) = 0;
};