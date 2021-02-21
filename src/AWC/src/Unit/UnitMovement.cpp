#include <AWC/Unit/UnitMovement.h>
#include <AWC/TilePattern/TilePatternI.h>


UnitMovement::UnitMovement(TilePatternIPtr tp) : tp_{tp}
{

}

bool UnitMovement::CanMove(Vector2 dest) const
{
    return tp_->IsTileInPattern(dest);
}

uint UnitMovement::GetMoveCostTo(Vector2 dest) const
{
    uint cost = -1;
    if(tp_->IsTileInPattern(dest))
        cost = tp_->GetTileCost(dest);
    return cost;
}

std::vector<Vector2> UnitMovement::GetPathTo(Vector2 dest) const
{
    return tp_->GetPathToTile(dest);
}

std::vector<Vector2> UnitMovement::GetTilesPosInReach() const
{
    return tp_->GetTilesPosInPattern();
}

