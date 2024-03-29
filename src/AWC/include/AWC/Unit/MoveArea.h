#pragma once

#include <AWC/AWCusing.h>

class MoveArea
{
public:
    MoveArea(AreaIPtr tp);

    bool CanMove(Vector2 dest) const;
    uint GetMoveCostTo(Vector2 dest) const;
    std::vector<Vector2> GetPathTo(Vector2 dest) const;
    std::vector<Vector2> GetTilesPosInReach() const;

private:
    AreaIPtr tp_;
};