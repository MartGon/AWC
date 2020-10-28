#include <AWC/TilePatternDescriptorDecorator.h>
#include <AWC/TilePatternConstraints.h>


// TPD Decorator

TilePatternDescriptorDecorator::TilePatternDescriptorDecorator(TilePatternDescriptorIPtr child) : child_{child}
{

}

// TPD FixedRange

TPDFixedRange::TPDFixedRange(TilePatternDescriptorIPtr child, unsigned int maxRange, unsigned int minRange)
     : maxRange_{maxRange}, minRange_{minRange}, TilePatternDescriptorDecorator{child}
{

}

TilePatternIPtr TPDFixedRange::DoCalculateTilePattern(Vector2 origin, 
    std::optional<Vector2> destination, const Map& map, const TilePatternConstraints& constraints)
{
    TilePatternIPtr result;
    TilePatternConstraints tpc = constraints;
    tpc.minRange = minRange_;
    tpc.maxRange = maxRange_;

    if(destination.has_value())
        result = child_->CalculateTilePattern(origin, destination.value(), map, tpc);
    else
        result = child_->CalculateTilePattern(origin, map, tpc);

    return result;
}

// TPD FixedCost

TPDFixedCost::TPDFixedCost(TilePatternDescriptorIPtr child, CostTableIPtr tileCostTable, CostTableIPtr unitCostTable) 
    : tileCostTable_{tileCostTable}, unitCostTable_{unitCostTable_},  TilePatternDescriptorDecorator{child}
{

}

TilePatternIPtr TPDFixedCost::DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
    const Map& map, const TilePatternConstraints& constraints)
{
    TilePatternIPtr result;
    TilePatternConstraints tpc{*tileCostTable_, *unitCostTable_, constraints.maxRange, constraints.minRange};

    if(destination.has_value())
        result = child_->CalculateTilePattern(origin, destination.value(), map, tpc);
    else
        result = child_->CalculateTilePattern(origin, map, tpc);

    return result;
}