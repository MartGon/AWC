#include <AWC/CostTable.h>

void CostTable::SetCost(unsigned int id, unsigned int cost)
{
    costMap_[id] = cost;
}

unsigned int CostTable::GetCost(unsigned int id) const
{
    return costMap_.at(id);
}