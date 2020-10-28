#pragma once

#include <Utils/Vector2.h>

#include <AWC/AWCusing.h>

#include <memory>

class TilePatternConstraints
{
public:
    TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);
    
    unsigned int GetTileCost(unsigned int id) const;

    unsigned int minRange;
    unsigned int maxRange;
private:
    CostTableIPtr tileCostTable;
    CostTableIPtr unitCostTable;
};