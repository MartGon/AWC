#include <AWC/CostTable.h>
#include <Utils/STLUtils.h>

// CostTable

void CostTable::SetCost(unsigned int id, unsigned int cost)
{
    costTable_.Set(id, cost);
}

void CostTable::SetDefaultCost(unsigned int cost)
{
    defaultCost_ = cost;
}

unsigned int CostTable::GetCost(unsigned int id) const
{
    auto cost = costTable_.ContainsId(id) ? costTable_.GetByIdCopy(id) : defaultCost_;

    return cost;
}