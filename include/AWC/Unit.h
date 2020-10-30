#pragma once

#include <string>

#include <Utils/Vector2.h>
#include <AWC/AWCusing.h>

class UnitType;

class Unit
{
public:
    Unit(const UnitType& unitType, const MovementDescPtr movementDesc, const std::vector<WeaponPtr> weapons);

    const std::string GetName() const;
    const uint GetId() const;

    // Movement
    UnitMovement CalculateMovement(const Map& map, Vector2 origin);

    // Attack
    bool CanAttack(UnitPtr unit);
    bool CanAttackWith(UnitPtr unit, uint weaponId);
    uint GetWeaponCount();

private:

    // Movement
    TilePatternConstraints GetMoveConstraints() const;

    // Attack
    bool IsWeaponIdValid(uint weaponId);

    const UnitType& unitType_;
    const MovementDescPtr moveDesc_;
    const std::vector<WeaponPtr> weapons_;
};