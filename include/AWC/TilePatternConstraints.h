#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>

#include <memory>

using CostTableIPtr = std::shared_ptr<CostTableI>;

class TilePatternConstraints
{
public:
    TilePatternConstraints(CostTableIPtr tileCostTable, CostTableIPtr unitCostTable, 
    unsigned int maxRange, unsigned int minRange = 0);
    
    unsigned int GetTileCost(unsigned int id) const;

    unsigned int minRange;
    unsigned int maxRange;
private:
    CostTableIPtr tileCostTable;
    CostTableIPtr unitCostTable;
};