#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Range.h>

class MovementDescType
{
public:
    MovementDescType(AreaDescIPtr tpd, Range range, CostTableIPtr tileCost,
        CostTableIPtr unitCost, uint maxGas);

    MovementDescPtr CreateMovementDesc() const;

    AreaDescIPtr tpd;
    Range range;
    CostTableIPtr tileCostTable;
    CostTableIPtr unitCostTable;
    uint maxGas;
};