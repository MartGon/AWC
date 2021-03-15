#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Area/AreaDescI.h>
#include <AWC/Range.h>

#include <memory>

class AreaDescDecorator : public AreaDescI
{
public:
    virtual ~AreaDescDecorator() {};
    AreaDescDecorator(AreaDescIPtr child);

protected:
    AreaDescIPtr child_;
};

class TPDStaticRange : public AreaDescDecorator
{
public:
    TPDStaticRange(AreaDescIPtr child, unsigned int maxRange, unsigned int minRange = 0);
    TPDStaticRange(AreaDescIPtr child, Range range);

private:
    AreaIPtr DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const AreaConstraints& constraints) override;

    Range range_;
};

class TPDStaticCost : public AreaDescDecorator
{
public:
    TPDStaticCost(AreaDescIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable);

private:
    AreaIPtr DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const AreaConstraints& constraints) override;

    CostTableIPtr tileCostTable_;
    CostTableIPtr unitCostTable_;
};