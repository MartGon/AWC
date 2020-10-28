#pragma once

#include <unordered_map>

class CostTableI
{
public:
    virtual ~CostTableI() {};

    virtual unsigned int GetCost(unsigned int id) const = 0;
};

class CostTable : public CostTableI
{
public:
    CostTable() {};
    CostTable(std::unordered_map<unsigned int, unsigned int> map) : costMap_{map} {};

    void SetCost(unsigned int id, unsigned int cost);
    unsigned int GetCost(unsigned int id) const override;

private:
    std::unordered_map<unsigned int, unsigned int> costMap_;
};

class FixedCostTable : public CostTableI
{
public:
    FixedCostTable(unsigned int cost);

    unsigned int GetCost(unsigned int id) const override;

private:
    unsigned int cost_;
};