#include <AWC/TilePatternConstraints.h>

TilePatternConstraints::TilePatternConstraints(const Map& map, const CostTable& tileCostTable, const CostTable& unitCostTable,
 unsigned int maxRange, unsigned int minRange) :
    minRange{minRange}, maxRange{maxRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}, map{map}
{

}