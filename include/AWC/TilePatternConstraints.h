#include <Utils/Vector2.h>
#include <AWC/CostTable.h>

class TilePatternConstraints
{
public:
    TilePatternConstraints(Vector2 origin, int minRange, int maxRange, CostTable tileCostTable, CostTable unitCostTable);

    Vector2 origin;
    unsigned int minRange;
    unsigned int maxRange;
    CostTable tileCostTable;
    CostTable unitCostTable;
};