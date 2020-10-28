#pragma once

#include <string>

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