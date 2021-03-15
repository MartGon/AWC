#pragma once

#include <AWC/AWCusing.h>

class UnitAttack
{
    friend class Unit;
public:

    bool CanAttack(Vector2 dest);
    std::vector<Vector2> GetTilesInRange();

private:
    UnitAttack(AreaIPtr tp);

    AreaIPtr tp_;
};