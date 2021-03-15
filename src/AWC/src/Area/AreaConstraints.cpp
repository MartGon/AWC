#include <AWC/Area/AreaConstraints.h>

#include <AWC/Map.h>
#include <AWC/CostTable.h>

AreaConstraints::AreaConstraints(CostTable tileCostTable, CostTable unitCostTable,
 unsigned int teamId, unsigned int maxRange, unsigned int minRange) :
    teamId{teamId}, range{maxRange, minRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}
{

}

AreaConstraints::AreaConstraints(CostTable tileCostTable, CostTable unitCostTable,
 unsigned int teamId, Range range) :
    teamId{teamId}, range{range}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}
{

}

unsigned int AreaConstraints::GetTileCost(unsigned int id) const
{
    return tileCostTable.GetCost(id);
}

unsigned int AreaConstraints::GetUnitCost(unsigned int id) const
{
    return unitCostTable.GetCost(id);
}