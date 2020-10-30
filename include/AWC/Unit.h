#pragma once

#include <string>

#include <Utils/Vector2.h>
#include <AWC/AWCusing.h>

class UnitType;

class Unit
{
public:
    Unit(const UnitType& unitType, const MovementDescPtr movementDesc, const WeaponPtr weapon);

    const std::string GetName() const;
    const uint GetId() const;

    UnitMovement CalculateMovement(const Map& map, Vector2 origin);

private:

    TilePatternConstraints GetMoveConstraints() const;

    const UnitType& unitType_;
    const MovementDescPtr moveDesc_;
    const WeaponPtr weapon_;
};