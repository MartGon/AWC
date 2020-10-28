#pragma once

#include <string>

#include <Utils/Vector2.h>
#include <AWC/AWCfwd.h>

class UnitType;

class Unit
{
public:
    Unit(const UnitType& unitType);

    const std::string GetName() const;

    UnitMovement CalculateMovement(const Map& map, const Vector2 origin) const;
    UnitMovement CalculateMovement(const Map& map, const Vector2 origin, const Vector2 dest) const;

private:
    const UnitType& unitType_;
};