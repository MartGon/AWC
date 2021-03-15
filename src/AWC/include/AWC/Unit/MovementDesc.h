#pragma once

#include <AWC/AWCusing.h>

class MovementDesc
{
friend class MovementDescType;

public:

    // Instance specific
    void Move(unsigned int cost);
    unsigned int GetCurrentGas();
    void IncreaseGas(unsigned int amount);
    void RefillGas();

    // Static data forwarded from Type
    AreaDescIPtr GetMovePattern();
    Range GetRange();
    unsigned int GetMaxGas();
    CostTableIPtr GetTileCostTable();
    CostTableIPtr GetUnitCostTable();

    unsigned int GetBaseTileCost(unsigned int tileId);

private:
    MovementDesc(const MovementDescType& moveType);

    unsigned int currentGas;
    const MovementDescType& moveType;
};