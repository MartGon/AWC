#include <AWC/Weapon.h>
#include <AWC/WeaponType.h>

Weapon::Weapon(const WeaponType& weaponType) : weaponType{weaponType}, currentAmmo{weaponType.maxAmmo}
{
    
}

// Instance specific

unsigned int Weapon::GetCurrentAmmo()
{
    return currentAmmo;
}

void Weapon::IncreaseAmmo(unsigned int amount)
{
    currentAmmo = std::min(currentAmmo + amount, GetMaxAmmo());
}

void Weapon::Use()
{
    currentAmmo = currentAmmo > 0 ? currentAmmo - 1 : 0;
}

void Weapon::RefillAmmo()
{
    currentAmmo = GetMaxAmmo();
}

bool Weapon::HasEnoughAmmo()
{
    return currentAmmo > 0;
}

// Static data forwarded from Type

TilePatternDescIPtr Weapon::GetAttackPattern()
{
    return weaponType.tpd;
}

Range Weapon::GetAttackRange()
{
    return weaponType.range;
}

bool Weapon::CanAttackUnit(unsigned int unitId)
{
    return weaponType.attackTable.CanAttack(unitId);
}

float Weapon::GetBaseDamageTo(unsigned int unitId)
{
    return weaponType.damageTable.GetDamageToUnit(unitId);
}

unsigned int Weapon::GetMaxAmmo()
{
    return weaponType.maxAmmo;
}