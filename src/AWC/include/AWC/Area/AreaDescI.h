#pragma once
#include <AWC/AWCusing.h>
#include <Utils.h>

#include <memory>
#include <optional>

class AreaDescI
{
public:
    virtual ~AreaDescI() {}

    AreaIPtr CalculateArea(const Map& map, Vector2 origin, const AreaConstraints& constraints);
    AreaIPtr CalculateArea(const Map& map, Vector2 origin, Vector2 destination, const AreaConstraints& constraints);

private:
    virtual AreaIPtr DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const AreaConstraints& constraints) = 0;
};