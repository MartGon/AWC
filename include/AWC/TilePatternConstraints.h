#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>

class TilePatternConstraints
{
public:
    TilePatternConstraints(const CostTableI& tileCostTable, const CostTableI& unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);
    
    unsigned int GetTileCost(unsigned int id) const;

    unsigned int minRange;
    unsigned int maxRange;
private:
    const CostTableI& tileCostTable;
    const CostTableI& unitCostTable;
};