#include <AWC/Area/AreaDescI.h>

AreaIPtr AreaDescI::CalculateArea(const Map& map, Vector2 origin, const AreaConstraints& constraints)
{
    return DoCalculateArea(map, origin, std::nullopt, constraints);
}

AreaIPtr AreaDescI::CalculateArea(const Map& map, Vector2 origin, Vector2 destination, 
    const AreaConstraints& constraints)
{
    return DoCalculateArea(map, origin, destination, constraints);
}