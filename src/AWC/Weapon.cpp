#include <AWC/Weapon.h>
#include <AWC/WeaponType.h>

Weapon::Weapon(const WeaponType& weaponType) : weaponType{weaponType}, currentAmmo{weaponType.maxAmmo}
{
    
}