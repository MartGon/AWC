#include <AWC/UnitMovement.h>
#include <AWC/TilePatternI.h>


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