#include <AWC/TilePatternConstraints.h>

#include <AWC/Map.h>
#include <AWC/CostTable.h>

TilePatternConstraints::TilePatternConstraints(const Map& map, const CostTable& tileCostTable, const CostTable& unitCostTable,
 unsigned int maxRange, unsigned int minRange) :
    minRange{minRange}, maxRange{maxRange}, tileCostTable{tileCostTable}, unitCostTable{unitCostTable}, map{map}
{

}

bool TilePatternConstraints::IsPositionValid(Vector2 pos) const
{
    return map.IsPositionValid(pos);
}

unsigned int TilePatternConstraints::GetTileCost(Vector2 pos) const
{
    auto tile = map.GetTile(pos);
    auto cost = tileCostTable.GetCost(tile->GetId());

    return cost;
}