#pragma once

#include <AWC/AWCusing.h>

class AttackArea
{
    friend class Unit;
public:

    bool CanAttack(Vector2 dest);
    std::vector<Vector2> GetTilesInRange();

private:
    AttackArea(AreaIPtr tp);

    AreaIPtr tp_;
};