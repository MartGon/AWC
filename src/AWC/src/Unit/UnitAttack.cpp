#include <AWC/Unit/UnitAttack.h>
#include <AWC/Area/AreaI.h>

UnitAttack::UnitAttack(AreaIPtr tp) : tp_{tp}
{

}

bool UnitAttack::CanAttack(Vector2 dest)
{
    return tp_->ContainsTile(dest);
}

std::vector<Vector2> UnitAttack::GetTilesInRange()
{
    return tp_->GetContainedTiles();
}