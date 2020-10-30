#include <AWC/Unit.h>
#include <AWC/UnitType.h>
#include <AWC/UnitMovement.h>

#include <AWC/TilePatternConstraints.h>

#include <AWC/MovementDescType.h>
#include <AWC/TilePatternDesc.h>

Unit::Unit(const UnitType& unitType, const MovementDescPtr movementDesc, const WeaponPtr weapon) : unitType_{unitType}, weapon_{weapon}, moveDesc_{movementDesc}
{

}

const std::string Unit::GetName() const
{
    return unitType_.GetName();
}

const uint Unit::GetId() const
{
    return unitType_.GetId();
}

UnitMovement Unit::CalculateMovement(const Map& map, Vector2 origin)
{
    auto moveDesc = unitType_.moveType_->tpd;
    auto moveConstraints = GetMoveConstraints();
    auto tp = moveDesc->CalculateTilePattern(origin, map, moveConstraints);
    return UnitMovement{tp};
}

// private

TilePatternConstraints Unit::GetMoveConstraints() const
{
    // Note: Some of these could be modified by buffs/powerups
    auto tileCost = unitType_.moveType_->tileCostTable;
    auto unitCost = unitType_.moveType_->unitCostTable;
    auto minRange = unitType_.moveType_->range.minRange;
    auto maxRange = unitType_.moveType_->range.maxRange;

    TilePatternConstraints tpc{tileCost, unitCost, maxRange, minRange};
    return tpc;
}