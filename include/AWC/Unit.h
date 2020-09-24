#pragma once

#include <string>

class UnitType;

class Unit
{
public:
    Unit(UnitType& unitType);

    const std::string GetName() const;

private:
    UnitType& unitType_;
};