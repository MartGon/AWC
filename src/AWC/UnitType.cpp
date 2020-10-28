#include <AWC/UnitType.h>
#include <AWC/Unit.h>

UnitType::UnitType(const std::string& name) : name_{name}
{

}

std::shared_ptr<Unit> UnitType::CreateUnit() const
{
    return std::make_shared<Unit>(Unit{*this});
}

const std::string UnitType::GetName() const
{
    return name_;
}

const UnitMovementDesc UnitType::GetUnitMovementDesc() const
{
    return unitMovementDesc_;
}