#pragma once

#include <string>

#include <Utils/Vector2.h>
#include <AWC/AWCusing.h>

class UnitType;

class Unit
{
friend class UnitType;

public:
    Unit(const UnitType& unitType, const MovementDescPtr movementDesc, const std::vector<WeaponPtr> weapons);

    const std::string GetName() const;
    const uint GetId() const;

    // Misc
    // TODO: Future release. Needs WeaponType and MovementType interface implementation.
    // UnitType GetUnitType(); 

    // Movement
    UnitMovement CalculateMovement(const Map& map, Vector2 origin);
    uint Move(unsigned int moveCost);

    // Attack

    UnitAttack CalculateAttack(unsigned int weaponId, const Map& map, Vector2 origin);

    // Can this unit attack given unit? Takes currentAmmo into account
    bool CanAttack(UnitPtr unit);

    // Can this unit attack a give unit with a specific? Takes current ammo into account
    bool CanAttackUnitWith(UnitPtr unit, uint weaponId);

    // How many weapons this unit has?
    uint GetWeaponCount();

    // How much damage the weapon of this unit deals to a given unit. Takes into account dmg mods but ignores defense mods.
    float GetDmgToUnit(unsigned int weaponId, UnitPtr unit);

    // Uses weapon with given id, consuming ammo.
    void UseWeapon(unsigned int weaponId);

    // Defense

    // How much damage takes this unit after defense mods?
    float GetDmgTaken(float incomingDmg);

    // Reduces this unit's health by a raw amount. Ignores defense mods.
    void TakeRawDamage(float incomingDmg);

    // Reduces this unit's health by a given amount. Takes defense mods into account.
    void TakeDamge(float incomingRawDmg);

private:

    // Movement
    TilePatternConstraints GetMoveConstraints() const;

    // Attack
    TilePatternConstraints GetAttackConstraints(unsigned int weaponId) const;
    bool IsWeaponIdValid(uint weaponId);

    void ThrowInvalidWeaponIdException(uint weaponId);

    const UnitType& unitType_;
    const MovementDescPtr moveDesc_;
    const std::vector<WeaponPtr> weapons_;
};