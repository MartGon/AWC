#include <AWC/UnitAttack.h>
#include <AWC/TilePattern/TilePatternI.h>

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