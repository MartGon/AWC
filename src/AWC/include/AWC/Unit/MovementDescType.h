#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Range.h>
#include <AWC/CostTable.h>

class MovementDescType
{
public:
    MovementDescType(AreaDescIPtr tpd, Range range, CostTable tileCost,
        CostTable unitCost, uint maxGas);

    MovementDescPtr CreateMovementDesc() const;

    AreaDescIPtr tpd;
    Range range;
    CostTable tileCostTable;
    CostTable unitCostTable;
    uint maxGas;
};