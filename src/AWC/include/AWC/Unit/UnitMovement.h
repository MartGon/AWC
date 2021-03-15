#pragma once

#include <AWC/AWCusing.h>

class UnitMovement
{
public:
    UnitMovement(AreaIPtr tp);

    bool CanMove(Vector2 dest) const;
    uint GetMoveCostTo(Vector2 dest) const;
    std::vector<Vector2> GetPathTo(Vector2 dest) const;
    std::vector<Vector2> GetTilesPosInReach() const;

private:
    AreaIPtr tp_;
};