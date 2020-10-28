#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescriptorI.h>

#include <memory>

using TilePatternConstraintsPtr = std::shared_ptr<TilePatternConstraints>;
using TilePatternDescriptorIPtr = std::shared_ptr<TilePatternDescriptorI>;

using CostTableIPtr = std::shared_ptr<CostTableI>;

class TilePatternDescriptorDecorator : public TilePatternDescriptorI
{
public:
    ~TilePatternDescriptorDecorator() {};
    TilePatternDescriptorDecorator(TilePatternDescriptorIPtr child);

protected:
    TilePatternDescriptorIPtr child_;
};

class TPDFixedRange : public TilePatternDescriptorDecorator
{
public:
    TPDFixedRange(TilePatternDescriptorIPtr child, unsigned int maxRange, unsigned int minRange = 0);

private:
    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) override;

    unsigned int minRange_;
    unsigned int maxRange_;
};

class TPDFixedCost : public TilePatternDescriptorDecorator
{
public:
    TPDFixedCost(TilePatternDescriptorIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable);

private:
    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) override;

    CostTableIPtr tileCostTable_;
    CostTableIPtr unitCostTable_;
};