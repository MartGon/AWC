#pragma once

#include <Utils/Table.h>

#include <AWC/AWCusing.h>
#include <AWC/Range.h>

class AttackTable
{
public:
    AttackTable(Table<bool> attackTable) : innerMap_{attackTable} {};
    AttackTable(std::unordered_map<uint, bool> attackableUnits) : innerMap_{attackableUnits} {};

    bool IsInTable(uint id) const;
    bool CanAttack(uint id) const;
    void SetAttackable(uint id, bool attackable);

private:
    Table<bool> innerMap_;
};

class DamageTable
{
public:
    DamageTable(Table<float> damageToUnits) : innerMap_{damageToUnits} {}
    DamageTable(std::unordered_map<uint, float> damageToUnits) : innerMap_{damageToUnits} {}

    bool IsInTable(uint id) const;
    float GetDamageToUnit(uint id) const;
    void SetDamageToUnit(uint id, float damage);

private:
    Table<float> innerMap_;
};

class WeaponType
{
public:
    WeaponType(AreaDescIPtr tpd, Range range, AttackTable attackTable, DamageTable damageTable, uint maxAmmo);

    WeaponPtr CreateWeapon() const;

    AreaDescIPtr tpd;
    Range range;

    // TODO: This two tables could become one and have AttackInfo
    AttackTable attackTable;
    DamageTable damageTable;

    unsigned int maxAmmo;
};