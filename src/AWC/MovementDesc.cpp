#include <AWC/MovementDesc.h>
#include <AWC/MovementDescType.h>

#include <AWC/CostTable.h>

MovementDesc::MovementDesc(const MovementDescType& moveType) : moveType{moveType}, currentGas{moveType.maxGas}
{

}

// Instance specific

void MovementDesc::Move(unsigned int cost)
{
    currentGas = currentGas >= cost ? currentGas - cost : 0;
}

unsigned int MovementDesc::GetCurrentGas()
{
    return currentGas;
}

void MovementDesc::IncreaseGas(unsigned int amount)
{
    currentGas = std::min(currentGas + amount, GetMaxGas());
}

void MovementDesc::RefillGas()
{
    currentGas = GetMaxGas();
}

// Static data forwarded from moveType

TilePatternDescIPtr MovementDesc::GetMovePattern()
{
    return moveType.tpd;
}

Range MovementDesc::GetRange()
{
    return moveType.range;
}

unsigned int MovementDesc::GetMaxGas()
{
    return moveType.maxGas;
}

CostTableIPtr MovementDesc::GetTileCostTable()
{
    return moveType.tileCostTable;
}

CostTableIPtr MovementDesc::GetUnitCostTable()
{
    return moveType.unitCostTable;
}

unsigned int MovementDesc::GetBaseTileCost(unsigned int tileId)
{
    return moveType.tileCostTable->GetCost(tileId);
}