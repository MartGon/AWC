#pragma once

#include <AWC/AWCusing.h>

class UnitMovement
{
public:
    UnitMovement(TilePatternIPtr tp);

    bool CanMove(Vector2 dest) const;
    std::vector<Vector2> GetPathTo(Vector2 dest) const;
    std::vector<Vector2> GetTilesPosInReach() const;

private:
    TilePatternIPtr tp_;
};