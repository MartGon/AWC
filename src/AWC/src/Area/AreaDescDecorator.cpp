#include <AWC/Area/AreaDescDecorator.h>
#include <AWC/Area/AreaConstraints.h>


// TPD Decorator

AreaDescDecorator::AreaDescDecorator(AreaDescIPtr child) : child_{child}
{

}

// TPD FixedRange

TPDStaticRange::TPDStaticRange(AreaDescIPtr child, unsigned int maxRange, unsigned int minRange)
     : range_{maxRange, minRange}, AreaDescDecorator{child}
{

}

TPDStaticRange::TPDStaticRange(AreaDescIPtr child, Range range)
     : range_{range}, AreaDescDecorator{child}
{

}

AreaIPtr TPDStaticRange::DoCalculateArea(const Map& map, Vector2 origin, 
    std::optional<Vector2> destination, const AreaConstraints& constraints)
{
    AreaIPtr result;
    AreaConstraints tpc = constraints;
    tpc.range = range_;

    if(destination.has_value())
        result = child_->CalculateArea(map, origin, destination.value(), tpc);
    else
        result = child_->CalculateArea(map, origin, tpc);

    return result;
}

// TPD FixedCost

TPDStaticCost::TPDStaticCost(AreaDescIPtr child, CostTable tileCostTable, CostTable unitCostTable) 
    : tileCostTable_{tileCostTable}, unitCostTable_{unitCostTable},  AreaDescDecorator{child}
{

}

AreaIPtr TPDStaticCost::DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
    const AreaConstraints& constraints)
{
    AreaIPtr result;
    AreaConstraints tpc{tileCostTable_, unitCostTable_, constraints.teamId, constraints.range};

    if(destination.has_value())
        result = child_->CalculateArea(map, origin, destination.value(), tpc);
    else
        result = child_->CalculateArea(map, origin, tpc);

    return result;
}