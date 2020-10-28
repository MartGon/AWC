#include <AWC/UnitMovement.h>

#include <AWC/TilePatternDescI.h>
#include <AWC/TilePatternConstraints.h>
#include <AWC/TilePatternI.h>

// UnitMovementDesc

UnitMovementDesc::UnitMovementDesc(TilePatternDescIPtr tilePatternDesc, CostTableIPtr tileCost, CostTableIPtr unitCost, unsigned int maxRange, unsigned int minRange) 
    : tilePatternDesc_{tilePatternDesc}, tileCost_{tileCost}, unitCost_{unitCost}, minRange_{minRange}, maxRange_{maxRange}
{

}

UnitMovement UnitMovementDesc::CalculateUnitMovement(const Map& map, Vector2 origin, Vector2 dest) const
{
    return tilePatternDesc_->CalculateTilePattern(origin, dest, map, GetMoveConstraints());
}

UnitMovement UnitMovementDesc::CalculateUnitMovement(const Map& map, Vector2 origin) const
{
    return tilePatternDesc_->CalculateTilePattern(origin, map, GetMoveConstraints());
}

TilePatternConstraints UnitMovementDesc::GetMoveConstraints() const
{
    return TilePatternConstraints{tileCost_, unitCost_, maxRange_, minRange_};
}

// UnitMovement

UnitMovement::UnitMovement(TilePatternIPtr tp) : tp_{tp}
{

}

bool UnitMovement::CanMove(Vector2 dest) const
{
    return tp_->IsTileInPattern(dest);
}

std::vector<Vector2> UnitMovement::GetPathTo(Vector2 dest) const
{
    return tp_->GetPathToTile(dest);
}

std::vector<Vector2> UnitMovement::GetTilesPosInReach() const
{
    return tp_->GetTilesPosInPattern();
}