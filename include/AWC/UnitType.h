#pragma once

#include <AWC/AWCusing.h>
#include <memory>
#include <string>

class Unit;

class UnitType
{
friend class Unit;

public:
    UnitType(uint id, const std::string& name, MovementDecTypePtr moveType, WeaponTypePtr weaponType);

    std::shared_ptr<Unit> CreateUnit() const;

    const std::string GetName() const;
    const uint GetId() const;

private:
    std::string name_;
    uint id_;

    MovementDecTypePtr moveType_;
    WeaponTypePtr weaponType_;
};