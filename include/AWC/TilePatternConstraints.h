#pragma once

#include <Utils/Vector2.h>

#include <AWC/AWCusing.h>
#include <AWC/Range.h>

#include <memory>

class TilePatternConstraints
{
public:
    TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);
    TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCosttable, Range range);
    
    unsigned int GetTileCost(unsigned int id) const;
    unsigned int GetUnitCost(unsigned int id) const;

    Range range;
private:
    CostTableIPtr tileCostTable;
    CostTableIPtr unitCostTable;
};