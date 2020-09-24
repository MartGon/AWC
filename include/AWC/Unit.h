#pragma once

#include <string>

class UnitType;

class Unit
{
public:
    Unit(const UnitType& unitType);

    const std::string GetName() const;

private:
    const UnitType& unitType_;
};