#include <AWC/Unit/MovementDescType.h>
#include <AWC/Unit/MovementDesc.h>

MovementDescType::MovementDescType(AreaDescIPtr tpd, Range range, CostTable tileCost, 
    CostTable unitCost, uint maxGas) : tpd{tpd}, range{range}, tileCostTable{tileCost}, 
    unitCostTable{unitCost}, maxGas{maxGas}
{

}

MovementDescPtr MovementDescType::CreateMovementDesc() const
{
    return MovementDescPtr{new MovementDesc{*this}};
}