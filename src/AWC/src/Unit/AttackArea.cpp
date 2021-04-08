#include <AWC/Unit/AttackArea.h>
#include <AWC/Area/AreaI.h>

AttackArea::AttackArea(AreaIPtr tp) : tp_{tp}
{

}

bool AttackArea::CanAttack(Vector2 dest)
{
    return tp_->ContainsTile(dest);
}

std::vector<Vector2> AttackArea::GetTilesInRange()
{
    return tp_->GetContainedTiles();
}