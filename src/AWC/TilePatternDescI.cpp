#include <AWC/TilePatternDescI.h>

TilePatternIPtr TilePatternDescI::CalculateTilePattern(Vector2 origin, const Map& map, const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, std::nullopt, map, constraints);
}

TilePatternIPtr TilePatternDescI::CalculateTilePattern(Vector2 origin, Vector2 destination, 
    const Map& map, const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, destination, map, constraints);
}