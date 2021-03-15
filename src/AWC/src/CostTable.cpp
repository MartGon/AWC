#include <AWC/CostTable.h>
#include <Utils/STLUtils.h>

// CostTable

void CostTable::SetCost(unsigned int id, unsigned int cost)
{
    costMap_[id] = cost;
}

unsigned int CostTable::GetCost(unsigned int id) const
{
    auto cost = UnorderedMapUtils::Contains(costMap_, id) ? costMap_.at(id) : defaultCost_;

    return cost;
}