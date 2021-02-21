#include <AWC/Unit/WeaponType.h>
#include <AWC/Unit/Weapon.h>

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

// DamageTable

bool DamageTable::IsInTable(uint id) const
{
    return innerMap_.find(id) != innerMap_.end();
}

float DamageTable::GetDamageToUnit(uint unitId) const
{
    return innerMap_.at(unitId);
}

void DamageTable::SetDamageToUnit(uint unitId, float damage)
{
    innerMap_[unitId] = damage;
}

// Weapon type

WeaponType::WeaponType(TilePatternDescIPtr tpd, Range range, AttackTable attackTable, DamageTable damageTable, uint maxAmmo) :
    tpd{tpd}, range{range}, attackTable{attackTable}, damageTable{damageTable}, maxAmmo{maxAmmo}

{

}

WeaponPtr WeaponType::CreateWeapon() const
{
    return WeaponPtr{new Weapon(*this)};
}