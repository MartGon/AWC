#include <AWC/TilePatternDescriptorI.h>

TilePatternPtr TilePatternDescriptorI::CalculateTilePattern(Vector2 origin, const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, std::nullopt, constraints);
}

TilePatternPtr TilePatternDescriptorI::CalculateTilePattern(Vector2 origin, Vector2 destination, 
    const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, destination, constraints);
}