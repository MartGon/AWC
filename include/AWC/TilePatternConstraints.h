#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>

class TilePatternConstraints
{
public:
    TilePatternConstraints(const Map& map, const CostTable& tileCostTable, const CostTable& unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);

    unsigned int minRange;
    unsigned int maxRange;
    const CostTable& tileCostTable;
    const CostTable& unitCostTable;
    const Map& map;
};