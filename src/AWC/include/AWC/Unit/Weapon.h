#pragma once

#include <AWC/AWCusing.h>

#include <AWC/Range.h>

class Weapon
{
friend class WeaponType;

public:

    // Instance specific
    unsigned int GetCurrentAmmo();
    void IncreaseAmmo(unsigned int amount);
    void RefillAmmo();
    void Use();
    bool HasEnoughAmmo();

    // Static data forwarded from Type
    AreaDescIPtr GetAttackPattern();
    Range GetAttackRange();
    bool CanAttackUnit(unsigned int unitId);
    float GetBaseDamageTo(unsigned int unitId);
    unsigned int GetMaxAmmo();

private:
    Weapon(const WeaponType& weaponType);

    const WeaponType& weaponType;
    unsigned int currentAmmo;
};