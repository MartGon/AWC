#pragma once

#include <string>

class Unit;

class UnitType
{
friend class Unit;

public:
    UnitType(const std::string& name);

    Unit* CreateUnit();

    const std::string GetName() const;
private:
    std::string name_;
};