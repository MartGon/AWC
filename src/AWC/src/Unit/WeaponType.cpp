#include <AWC/Unit/WeaponType.h>
#include <AWC/Unit/Weapon.h>

// AttackTable

bool AttackTable::IsInTable(uint id) const
{
    return innerMap_.ContainsId(id);
}

bool AttackTable::CanAttack(uint id) const
{
    return IsInTable(id) && innerMap_.GetByIdCopy(id);
}

void AttackTable::SetAttackable(uint id, bool attackable)
{
    innerMap_.Set(id, attackable);
}

// DamageTable

bool DamageTable::IsInTable(uint id) const
{
    return innerMap_.ContainsId(id);
}

float DamageTable::GetDamageToUnit(uint unitId) const
{
    return innerMap_.GetByIdCopy(unitId);
}

void DamageTable::SetDamageToUnit(uint unitId, float damage)
{
    innerMap_.Set(unitId, damage);
}

// Weapon type

WeaponType::WeaponType(AreaDescIPtr tpd, Range range, AttackTable attackTable, DamageTable damageTable, uint maxAmmo) :
    tpd{tpd}, range{range}, attackTable{attackTable}, damageTable{damageTable}, maxAmmo{maxAmmo}

{

}

WeaponPtr WeaponType::CreateWeapon() const
{
    return WeaponPtr{new Weapon(*this)};
}