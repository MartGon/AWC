#pragma once

#include <AWC/UnitMovement.h>

#include <memory>
#include <string>

class Unit;

class UnitType
{
friend class Unit;

public:
    UnitType(const std::string& name, UnitMovementDesc unitMovementDesc);

    std::shared_ptr<Unit> CreateUnit() const;

    const std::string GetName() const;
    const UnitMovementDesc GetUnitMovementDesc() const;

private:

    UnitMovementDesc unitMovementDesc_;
    std::string name_;
};