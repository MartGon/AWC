#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescI.h>

#include <memory>

using TilePatternConstraintsPtr = std::shared_ptr<TilePatternConstraints>;
using TilePatternDescIPtr = std::shared_ptr<TilePatternDescI>;

using CostTableIPtr = std::shared_ptr<CostTableI>;

class TilePatternDescDecorator : public TilePatternDescI
{
public:
    ~TilePatternDescDecorator() {};
    TilePatternDescDecorator(TilePatternDescIPtr child);

protected:
    TilePatternDescIPtr child_;
};

class TPDFixedRange : public TilePatternDescDecorator
{
public:
    TPDFixedRange(TilePatternDescIPtr child, unsigned int maxRange, unsigned int minRange = 0);

private:
    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) override;

    unsigned int minRange_;
    unsigned int maxRange_;
};

class TPDFixedCost : public TilePatternDescDecorator
{
public:
    TPDFixedCost(TilePatternDescIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable);

private:
    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) override;

    CostTableIPtr tileCostTable_;
    CostTableIPtr unitCostTable_;
};