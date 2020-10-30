#pragma once

#include <AWC/AWCfwd.h>

class Weapon
{
friend class WeaponType;
public:

    const WeaponType& weaponType;
    unsigned int currentAmmo;

private:
    Weapon(const WeaponType& weaponType);
};