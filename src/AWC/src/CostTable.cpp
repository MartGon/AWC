#include <AWC/CostTable.h>

// CostTable

void CostTable::SetCost(unsigned int id, unsigned int cost)
{
    costMap_[id] = cost;
}

unsigned int CostTable::GetCost(unsigned int id) const
{
    return costMap_.at(id);
}

// FixedCostTable

FixedCostTable::FixedCostTable(unsigned int cost) : cost_{cost}
{
    
}

unsigned int FixedCostTable::GetCost(unsigned int id) const
{
    return cost_;
}