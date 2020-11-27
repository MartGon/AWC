#include <AWC/TilePatternConstraints.h>

#include <AWC/Map.h>
#include <AWC/CostTable.h>

TilePatternConstraints::TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable,
 unsigned int teamId, unsigned int maxRange, unsigned int minRange) :
    teamId{teamId}, range{maxRange, minRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}
{

}

TilePatternConstraints::TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable,
 unsigned int teamId, Range range) :
    teamId{teamId}, range{range}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}
{

}

unsigned int TilePatternConstraints::GetTileCost(unsigned int id) const
{
    return tileCostTable->GetCost(id);
}

unsigned int TilePatternConstraints::GetUnitCost(unsigned int id) const
{
    return unitCostTable->GetCost(id);
}