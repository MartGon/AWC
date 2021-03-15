#include <AWC/Unit/UnitMovement.h>
#include <AWC/Area/AreaI.h>


UnitMovement::UnitMovement(AreaIPtr tp) : tp_{tp}
{

}

bool UnitMovement::CanMove(Vector2 dest) const
{
    return tp_->ContainsTile(dest);
}

uint UnitMovement::GetMoveCostTo(Vector2 dest) const
{
    uint cost = -1;
    if(tp_->ContainsTile(dest))
        cost = tp_->GetTileCost(dest);
    return cost;
}

std::vector<Vector2> UnitMovement::GetPathTo(Vector2 dest) const
{
    return tp_->GetPathToTile(dest);
}

std::vector<Vector2> UnitMovement::GetTilesPosInReach() const
{
    return tp_->GetContainedTiles();
}

