#include <AWC/TilePatternDescDecorator.h>
#include <AWC/TilePatternConstraints.h>


// TPD Decorator

TilePatternDescDecorator::TilePatternDescDecorator(TilePatternDescIPtr child) : child_{child}
{

}

// TPD FixedRange

TPDFixedRange::TPDFixedRange(TilePatternDescIPtr child, unsigned int maxRange, unsigned int minRange)
     : range_{maxRange, minRange}, TilePatternDescDecorator{child}
{

}

TPDFixedRange::TPDFixedRange(TilePatternDescIPtr child, Range range)
     : range_{range}, TilePatternDescDecorator{child}
{

}

TilePatternIPtr TPDFixedRange::DoCalculateTilePattern(const Map& map, Vector2 origin, 
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

TPDFixedCost::TPDFixedCost(TilePatternDescIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable) 
    : tileCostTable_{tileCostTable}, unitCostTable_{unitCostTable_},  TilePatternDescDecorator{child}
{

}

TilePatternIPtr TPDFixedCost::DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
    const TilePatternConstraints& constraints)
{
    TilePatternIPtr result;
    TilePatternConstraints tpc{tileCostTable_, unitCostTable_, constraints.range};

    if(destination.has_value())
        result = child_->CalculateTilePattern(map, origin, destination.value(), tpc);
    else
        result = child_->CalculateTilePattern(map, origin, tpc);

    return result;
}