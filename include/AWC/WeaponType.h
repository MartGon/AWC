#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Range.h>

class AttackTable
{
public:
    AttackTable(std::unordered_map<uint, bool> attackableUnits) : innerMap_{attackableUnits} {};

    bool IsInTable(uint id) const;
    bool CanAttack(uint id) const;
    void SetAttackable(uint id, bool attackable);

private:
    std::unordered_map<uint, bool> innerMap_;
};

class DamageTable
{
public:
    DamageTable(std::unordered_map<uint, float> damageToUnits) : innerMap_{damageToUnits} {}

    bool IsInTable(uint id) const;
    float GetDamageToUnit(uint id) const;
    void SetDamageToUnit(uint id, float damage);

private:
    std::unordered_map<uint, float> innerMap_;
};

class WeaponType
{
public:
    WeaponType(TilePatternDescIPtr tpd, Range range, AttackTable attackTable, DamageTable damageTable, uint maxAmmo);

    WeaponPtr CreateWeapon() const;

    TilePatternDescIPtr tpd;
    Range range;
    AttackTable attackTable;
    DamageTable damageTable;

    unsigned int maxAmmo;
};