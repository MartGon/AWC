#include <AWC/TilePatternConstraints.h>

TilePatternConstraints::TilePatternConstraints(const Map& map, unsigned int minRange, unsigned int maxRange, const CostTable& tileCostTable, 
    const CostTable& unitCostTable) :
    minRange{minRange}, maxRange{maxRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}, map{map}
{

}