#pragma once

#include <string>

#include <Utils.h>
#include <AWC/AWCusing.h>
#include <AWC/Event.h>
#include <AWC/Entity.h>

class UnitType;

namespace UnitNS
{
    enum Flag : uint
    {
        NONE        = 0x0,
        MOVED       = 0x1,
        ATTACKED    = 0x2,
        DEAD        = 0x4
    };

    enum StatType : uint
    {
        HEALTH,
        GAS,
        AMMO
    };

    
}

class Unit
{
friend class UnitType;

public:
    const std::string GetName() const;
    const uint GetTypeId() const;
    Player& GetOwner() const;

    Entity::GUID GetGUID() const;

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
    uint GetCurrentGas() const;

    // Returns whether this unit can move this turn
    bool CanMove() const;

    // Attack

    // TODO: Move this out of Unit. Reduces coupling with Map
    // Returns information about the attacks this unit can perform with a given weapon
    UnitAttack CalculateAttack(unsigned int weaponId, const Map& map, Vector2 origin);

    // Returns whether this unit can attack this turn
    bool CanAttack() const;

    // Can this unit attack given unit? Takes currentAmmo into account
    bool CanAttack(UnitPtr unit) const;

    // Can this unit attack a given unit with a specific weapon? Takes current ammo into account
    bool CanAttackUnitWith(UnitPtr unit, uint weaponId) const;

    // How many weapons this unit has?
    uint GetWeaponCount() const;

    // How much damage the weapon of this unit deals to a given unit. Takes into account dmg mods but ignores defense mods.
    float GetDmgToUnit(unsigned int weaponId, UnitPtr unit) const;

    // Uses weapon with given id, consuming ammo.
    void UseWeapon(unsigned int weaponId);

    // Returns how much ammo a given weapon has left
    uint GetWeaponAmmo(unsigned int weaponId) const;

    // Defense

    // How much damage takes this unit after defense mods?
    float GetDmgTaken(float incomingDmg);

    // Reduces this unit's health by a raw amount. Ignores defense mods.
    void TakeRawDamage(float incomingDmg);

    // Reduces this unit's health by a given amount. Takes defense mods into account.
    void TakeDamage(float incomingRawDmg);

    // Returns unit current health
    float GetHealth();

    // Returns whether this unit is dead (health <= 0)
    bool IsDead();

    // State

    // Callback to be called on pass turn
    void OnPassTurn(Turn& turn);

    // Sets a UnitFlag
    void SetFlag(UnitNS::Flag flag);

    // Clears a UnitFlag
    void RemoveFlag(UnitNS::Flag flag);

    // Checks a UnitFlag
    bool HasFlag(UnitNS::Flag flag) const;

    // Events
    void RegisterHandlers(Event::Subject& subject);

private:
    Unit(uint id, const UnitType& unitType, const MovementDescPtr movementDesc, const std::vector<WeaponPtr> weapons, Player& ownerId);

    // Movement
    // TODO: This should become public once CalculateMovement does
    AreaConstraints GetMoveConstraints() const;

    // Attack
    // TODO: This should become public once CalculateAttack does
    AreaConstraints GetAttackConstraints(unsigned int weaponId) const;
    bool IsWeaponIdValid(uint weaponId) const;

    // State

    // Utils
    void ThrowInvalidWeaponIdException(uint weaponId) const;

    // Member variables
    uint id_;

    // State
    float health = 100;
    uint flags;

    Player& owner_;
    const UnitType& unitType_;
    const MovementDescPtr moveDesc_;
    const std::vector<WeaponPtr> weapons_;
};