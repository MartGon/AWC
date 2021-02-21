#include <AWC/TilePattern/TilePatternDescDecorator.h>
#include <AWC/TilePattern/TilePatternConstraints.h>


// TPD Decorator

TilePatternDescDecorator::TilePatternDescDecorator(TilePatternDescIPtr child) : child_{child}
{

}

// TPD FixedRange

TPDStaticRange::TPDStaticRange(TilePatternDescIPtr child, unsigned int maxRange, unsigned int minRange)
     : range_{maxRange, minRange}, TilePatternDescDecorator{child}
{

}

TPDStaticRange::TPDStaticRange(TilePatternDescIPtr child, Range range)
     : range_{range}, TilePatternDescDecorator{child}
{

}

TilePatternIPtr TPDStaticRange::DoCalculateTilePattern(const Map& map, Vector2 origin, 
    std::optional<Vector2> destination, const TilePatternConstraints& constraints)
{
    TilePatternIPtr result;
    TilePatternConstraints tpc = constraints;
    tpc.range = range_;

    if(destination.has_value())
        result = child_->CalculateTilePattern(map, origin, destination.value(), tpc);
    else
        result = child_->CalculateTilePattern(map, origin, tpc);

    return result;
}

// TPD FixedCost

TPDStaticCost::TPDStaticCost(TilePatternDescIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable) 
    : tileCostTable_{tileCostTable}, unitCostTable_{unitCostTable},  TilePatternDescDecorator{child}
{

}

TilePatternIPtr TPDStaticCost::DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
    const TilePatternConstraints& constraints)
{
    TilePatternIPtr result;
    TilePatternConstraints tpc{tileCostTable_, unitCostTable_, constraints.teamId, constraints.range};

    if(destination.has_value())
        result = child_->CalculateTilePattern(map, origin, destination.value(), tpc);
    else
        result = child_->CalculateTilePattern(map, origin, tpc);

    return result;
}