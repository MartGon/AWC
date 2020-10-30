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

class WeaponType
{
public:
    WeaponType(TilePatternDescIPtr tpd, Range range, AttackTable attackTable, uint maxAmmo, uint damage);

    WeaponPtr CreateWeapon() const;

    TilePatternDescIPtr tpd;
    Range range;
    AttackTable attackTable;

    unsigned int maxAmmo;
    unsigned int damage;
};