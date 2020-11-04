#include <AWC/TilePatternDescI.h>

TilePatternIPtr TilePatternDescI::CalculateTilePattern(const Map& map, Vector2 origin, const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(map, origin, std::nullopt, constraints);
}

TilePatternIPtr TilePatternDescI::CalculateTilePattern(const Map& map, Vector2 origin, Vector2 destination, 
    const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(map, origin, destination, constraints);
}