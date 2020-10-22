#include <AWC/TilePatternDescriptorI.h>

TilePatternIPtr TilePatternDescriptorI::CalculateTilePattern(Vector2 origin, const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, std::nullopt, constraints);
}

TilePatternIPtr TilePatternDescriptorI::CalculateTilePattern(Vector2 origin, Vector2 destination, 
    const TilePatternConstraints& constraints)
{
    return DoCalculateTilePattern(origin, destination, constraints);
}