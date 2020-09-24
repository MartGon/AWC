#include <AWC/Unit.h>
#include <AWC/UnitType.h>

Unit::Unit(const UnitType& unitType) : unitType_{unitType}
{

}

const std::string Unit::GetName() const
{
    return unitType_.GetName();
}