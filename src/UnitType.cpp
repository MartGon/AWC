#include <AWC/UnitType.h>
#include <AWC/Unit.h>

UnitType::UnitType(const std::string& name) : name_{name}
{

}

Unit* UnitType::CreateUnit()
{
    return new Unit(*this);
}

const std::string UnitType::GetName() const
{
    return name_;
}