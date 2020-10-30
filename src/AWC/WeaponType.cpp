#include <AWC/WeaponType.h>
#include <AWC/Weapon.h>

// AttackTable

bool AttackTable::IsInTable(uint id) const
{
    return innerMap_.find(id) != innerMap_.end();
}

bool AttackTable::CanAttack(uint id) const
{
    return IsInTable(id) && innerMap_.at(id);
}

void AttackTable::SetAttackable(uint id, bool attackable)
{
    innerMap_[id] = attackable;
}

// Weapon type

WeaponType::WeaponType(TilePatternDescIPtr tpd, Range range, AttackTable attackTable, uint maxAmmo, uint damage) :
    tpd{tpd}, range{range}, attackTable{attackTable}, maxAmmo{maxAmmo}, damage{damage}

{

}

WeaponPtr WeaponType::CreateWeapon() const
{
    return WeaponPtr{new Weapon(*this)};
}