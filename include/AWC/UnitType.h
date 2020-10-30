#pragma once

#include <AWC/AWCusing.h>
#include <memory>
#include <string>

class Unit;

class UnitType
{
public:
    UnitType(uint id, const std::string& name, MovementDecTypePtr moveType, std::vector<WeaponTypePtr> weaponTypes);

    std::shared_ptr<Unit> CreateUnit() const;

    const std::string GetName() const;
    const uint GetId() const;

private:

    std::vector<WeaponPtr> GetWeapons() const;

    std::string name_;
    uint id_;

    MovementDecTypePtr moveType_;
    std::vector<WeaponTypePtr> weaponTypes_;
};