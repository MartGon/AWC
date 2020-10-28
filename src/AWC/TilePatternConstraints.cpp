#include <AWC/TilePatternConstraints.h>

#include <AWC/Map.h>
#include <AWC/CostTable.h>

TilePatternConstraints::TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable,
 unsigned int maxRange, unsigned int minRange) :
    minRange{minRange}, maxRange{maxRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}
{

}

unsigned int TilePatternConstraints::GetTileCost(unsigned int id) const
{
    return tileCostTable->GetCost(id);
}