#include <AWC/MovementDescType.h>
#include <AWC/MovementDesc.h>

MovementDescType::MovementDescType(TilePatternDescIPtr tpd, Range range, CostTableIPtr tileCost, 
    CostTableIPtr unitCost, uint maxGas) : tpd{tpd}, range{range}, tileCostTable{tileCost}, 
    unitCostTable{unitCost}, maxGas{maxGas}
{

}

MovementDescPtr MovementDescType::CreateMovementDesc() const
{
    return MovementDescPtr{new MovementDesc{*this}};
}