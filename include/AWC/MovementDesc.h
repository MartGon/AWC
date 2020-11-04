#pragma once

#include <AWC/AWCusing.h>

class MovementDesc
{
friend class MovementDescType;

public:

    // Instance specific
    void Move(unsigned int cost);
    unsigned int GetCurrentGas();
    void RefillGas();

    // Static data forwarded from moveType
    TilePatternDescIPtr GetMovePattern();
    Range GetRange();
    unsigned int GetMaxGas();
    CostTableIPtr GetTileCostTable();
    CostTableIPtr GetUnitCostTable();

    unsigned int GetBaseTileCost(unsigned int tileId);

private:

    unsigned int currentGas;
    const MovementDescType& moveType;

    MovementDesc(const MovementDescType& moveType);
};