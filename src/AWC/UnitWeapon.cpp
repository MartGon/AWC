#include <AWC/UnitWeapon.h>

#include <AWC/TilePatternDesc.h>
#include <AWC/TilePatternConstraints.h>
#include <AWC/TilePattern.h>
#include <AWC/CostTable.h>

// UnitWeapon

UnitWeapon::UnitWeapon(TilePatternDescIPtr tpd, uint maxRange, uint minRange) :
    tilePatternDesc_{tpd}, maxRange_{maxRange}, minRange_{minRange}
{

}

UnitAttack UnitWeapon::CalculateUnitAttack(const Map& map, Vector2 origin, Vector2 dest)
{
    auto ac = GetAttackConstraints();
    auto tp = tilePatternDesc_->CalculateTilePattern(origin, dest, map, ac);
    return tp;
}

UnitAttack UnitWeapon::CalculateUnitAttack(const Map& map, Vector2 origin)
{
    auto ac = GetAttackConstraints();
    auto tp = tilePatternDesc_->CalculateTilePattern(origin, map, ac);
    return tp;
}

TilePatternConstraints UnitWeapon::GetAttackConstraints()
{
    CostTableIPtr unitCT{new FixedCostTable{0}};
    CostTableIPtr tileCT{new FixedCostTable{1}};

    TilePatternConstraints tpc{tileCT, unitCT, maxRange_, minRange_};
    return tpc;
}

// UnitAttack

UnitAttack::UnitAttack(TilePatternIPtr tp) : tp_{tp}
{

}

bool UnitAttack::CanAttack(Vector2 dest)
{
    return tp_->IsTileInPattern(dest);
}

std::vector<Vector2> UnitAttack::GetTilesInRange()
{
    return tp_->GetTilesPosInPattern();
}