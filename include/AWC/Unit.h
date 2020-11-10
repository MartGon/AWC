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

    // TODO: Move this out of Unit. Reduces coupling with Map
    // Returns information about the moves this unit can perform.
    UnitMovement CalculateMovement(const Map& map, Vector2 origin);

    // Reduces currentGas by a given moveCost. Can also trigger movement events or be affected by powerups
    void Move(unsigned int moveCost);

    // Returns how much gas this Unit has left
    uint GetCurrentGas();

    // Attack

    // TODO: Move this out of Unit. Reduces coupling with Map
    // Returns information about the attacks this unit can perform with a given weapon
    UnitAttack CalculateAttack(unsigned int weaponId, const Map& map, Vector2 origin);

    // Can this unit attack given unit? Takes currentAmmo into account
    bool CanAttack(UnitPtr unit);

    // Can this unit attack a given unit with a specific weapon? Takes current ammo into account
    bool CanAttackUnitWith(UnitPtr unit, uint weaponId);

    // How many weapons this unit has?
    uint GetWeaponCount();

    // How much damage the weapon of this unit deals to a given unit. Takes into account dmg mods but ignores defense mods.
    float GetDmgToUnit(unsigned int weaponId, UnitPtr unit);

    // Uses weapon with given id, consuming ammo.
    void UseWeapon(unsigned int weaponId);

    // Returns how much ammo a given weapon has left
    uint GetWeaponAmmo(unsigned int weaponId);

    // Defense

    // How much damage takes this unit after defense mods?
    float GetDmgTaken(float incomingDmg);

    // Reduces this unit's health by a raw amount. Ignores defense mods.
    void TakeRawDamage(float incomingDmg);

    // Reduces this unit's health by a given amount. Takes defense mods into account.
    void TakeDamge(float incomingRawDmg);

private:

    // Movement
    // TODO: This should become public once CalculateMovement does
    TilePatternConstraints GetMoveConstraints() const;

    // Attack
    // TODO: This should become public once CalculateAttack does
    TilePatternConstraints GetAttackConstraints(unsigned int weaponId) const;
    bool IsWeaponIdValid(uint weaponId);

    // Utils
    void ThrowInvalidWeaponIdException(uint weaponId);

    // TODO: Add GUID.

    const UnitType& unitType_;
    const MovementDescPtr moveDesc_;
    const std::vector<WeaponPtr> weapons_;
};