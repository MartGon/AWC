#pragma once

#include <AWC/AWCusing.h>

class UnitAttack
{
    friend class Unit;
public:

    bool CanAttack(Vector2 dest);
    std::vector<Vector2> GetTilesInRange();

private:
    UnitAttack(TilePatternIPtr tp);

    TilePatternIPtr tp_;
};