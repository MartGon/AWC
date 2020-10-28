#include <AWC/Unit.h>
#include <AWC/UnitType.h>

Unit::Unit(const UnitType& unitType) : unitType_{unitType}
{

}

const std::string Unit::GetName() const
{
    return unitType_.GetName();
}

UnitMovement Unit::CalculateMovement(const Map& map, const Vector2 origin) const
{
    auto umDesc = unitType_.GetUnitMovementDesc();
    return umDesc.CalculateUnitMovement(map, origin);
}

UnitMovement Unit::CalculateMovement(const Map& map, const Vector2 origin, const Vector2 dest) const
{
    auto umDesc = unitType_.GetUnitMovementDesc();
    return umDesc.CalculateUnitMovement(map, origin, dest);
}