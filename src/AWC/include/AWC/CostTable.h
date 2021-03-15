#pragma once

#include <unordered_map>

class CostTable
{
public:
    CostTable() : defaultCost_{std::numeric_limits<unsigned int>::max()} {};
    CostTable(unsigned int defaultCost) : defaultCost_{defaultCost} {};
    CostTable(std::unordered_map<unsigned int, unsigned int> map, unsigned int defaultCost) : costMap_{map}, defaultCost_{defaultCost} {};

    void SetCost(unsigned int id, unsigned int cost);
    unsigned int GetCost(unsigned int id) const;

private:
    std::unordered_map<unsigned int, unsigned int> costMap_;
    unsigned int defaultCost_;
};