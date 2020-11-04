#pragma once

#include <AWC/AWCusing.h>
#include <AWC/TilePatternDescI.h>
#include <AWC/Range.h>

#include <memory>

class TilePatternDescDecorator : public TilePatternDescI
{
public:
    virtual ~TilePatternDescDecorator() {};
    TilePatternDescDecorator(TilePatternDescIPtr child);

protected:
    TilePatternDescIPtr child_;
};

class TPDFixedRange : public TilePatternDescDecorator
{
public:
    TPDFixedRange(TilePatternDescIPtr child, unsigned int maxRange, unsigned int minRange = 0);
    TPDFixedRange(TilePatternDescIPtr child, Range range);

private:
    TilePatternIPtr DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const TilePatternConstraints& constraints) override;

    Range range_;
};

class TPDFixedCost : public TilePatternDescDecorator
{
public:
    TPDFixedCost(TilePatternDescIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable);

private:
    TilePatternIPtr DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const TilePatternConstraints& constraints) override;

    CostTableIPtr tileCostTable_;
    CostTableIPtr unitCostTable_;
};