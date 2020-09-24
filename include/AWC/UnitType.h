#pragma once

#include <memory>
#include <string>

class Unit;

class UnitType
{
friend class Unit;

public:
    UnitType(const std::string& name);

    std::shared_ptr<Unit> CreateUnit() const;

    const std::string GetName() const;
private:
    std::string name_;
};