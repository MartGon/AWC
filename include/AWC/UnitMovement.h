#pragma once

#include <AWC/AWCusing.h>

#include <Utils/Vector2.h>

#include <memory>
#include <vector>

// TODO: What if the unit movement range is increased by a buff or reduces by a weather type?


class UnitMovementDesc
{
public:
    UnitMovementDesc(TilePatternDescIPtr tilePatternDesc, CostTableIPtr tileCost, CostTableIPtr unitCost, unsigned int maxRange, unsigned int minRange = 0);

    UnitMovement CalculateUnitMovement(const Map& map, Vector2 origin, Vector2 dest) const;
    UnitMovement CalculateUnitMovement(const Map& map, Vector2 origin) const;

private:

    TilePatternConstraints GetMoveConstraints() const;

    TilePatternDescIPtr tilePatternDesc_;
    CostTableIPtr tileCost_;
    CostTableIPtr unitCost_;
    unsigned int minRange_;
    unsigned int maxRange_;
};

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