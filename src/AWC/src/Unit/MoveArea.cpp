#include <AWC/Unit/MoveArea.h>
#include <AWC/Area/AreaI.h>


MoveArea::MoveArea(AreaIPtr tp) : tp_{tp}
{

}

bool MoveArea::CanMove(Vector2 dest) const
{
    return tp_->ContainsTile(dest);
}

uint MoveArea::GetMoveCostTo(Vector2 dest) const
{
    uint cost = -1;
    if(tp_->ContainsTile(dest))
        cost = tp_->GetTileCost(dest);
    return cost;
}

std::vector<Vector2> MoveArea::GetPathTo(Vector2 dest) const
{
    return tp_->GetPathToTile(dest);
}

std::vector<Vector2> MoveArea::GetTilesPosInReach() const
{
    return tp_->GetContainedTiles();
}

