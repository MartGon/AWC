#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>

class TilePatternConstraints
{
public:
    TilePatternConstraints(const Map& map, const CostTable& tileCostTable, const CostTable& unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);

    bool IsPositionValid(Vector2 pos);
    unsigned int GetTileCost(Vector2 pos);

    const Map& GetMap() {return map;}

    const unsigned int minRange;
    const unsigned int maxRange;

private:
    const CostTable& tileCostTable;
    const CostTable& unitCostTable;
    const Map& map;
};