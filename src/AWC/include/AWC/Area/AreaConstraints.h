#pragma once

#include <Utils.h>

#include <AWC/AWCusing.h>
#include <AWC/Range.h>
#include <AWC/CostTable.h>

#include <memory>

class AreaConstraints
{
public:
    AreaConstraints(CostTable tileCostTable, CostTable unitCostTable, uint teamId,
    unsigned int maxRange, unsigned int minRange = 0);
    AreaConstraints(CostTable tileCostTable, CostTable unitCosttable, uint teamId, Range range);
    
    unsigned int GetTileCost(unsigned int id) const;
    unsigned int GetUnitCost(unsigned int id) const;

    const uint teamId;
    Range range;
private:
    CostTable tileCostTable;
    CostTable unitCostTable;
};