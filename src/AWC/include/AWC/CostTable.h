#pragma once

#include <unordered_map>
#include <Utils/Table.h>

class CostTable
{
public: 
    CostTable() : defaultCost_{std::numeric_limits<unsigned int>::max()} {};
    CostTable(unsigned int defaultCost) : defaultCost_{defaultCost} {};
    CostTable(std::unordered_map<unsigned int, unsigned int> map, unsigned int defaultCost) : costTable_{map}, defaultCost_{defaultCost} {};
    CostTable(Table<unsigned int> table, unsigned int defaultCost) : costTable_{table}, defaultCost_{defaultCost} {};

    void SetCost(unsigned int id, unsigned int cost);
    void SetDefaultCost(unsigned int cost);
    unsigned int GetCost(unsigned int id) const;

private:
    Table<unsigned int> costTable_;
    unsigned int defaultCost_;
};