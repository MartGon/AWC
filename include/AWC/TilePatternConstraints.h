#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>

class TilePatternConstraints
{
public:
    TilePatternConstraints(const Map& map, const CostTableI& tileCostTable, const CostTableI& unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);

    bool IsPositionValid(Vector2 pos) const;
    unsigned int GetTileCost(Vector2 pos) const;

    const Map& GetMap() {return map;}

    const unsigned int minRange;
    const unsigned int maxRange;

private:
    const CostTableI& tileCostTable;
    const CostTableI& unitCostTable;
    const Map& map;
};