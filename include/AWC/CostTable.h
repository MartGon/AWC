
#include <unordered_map>

class CostTable
{
public:
    CostTable() {};
    CostTable(std::unordered_map<unsigned int, unsigned int> map) : costMap_{map} {};

    void SetCost(unsigned int id, unsigned int cost);
    unsigned int GetCost(unsigned int id) const;

private:
    std::unordered_map<unsigned int, unsigned int> costMap_;
};