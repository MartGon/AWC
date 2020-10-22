#include <AWC/TilePatternDescriptorComp.h>

// Union

TilePatternDescriptorUnion::TilePatternDescriptorUnion(TilePatternDescriptorPtr a, TilePatternDescriptorPtr b) : a_{a}, b_{b}
{

}

TilePatternPtr TilePatternDescriptorUnion::DoCalculateTilePattern(Vector2 origin, 
    std::optional<Vector2> destination, const TilePatternConstraints& constraints)
{
    // There is redundancy here
    // It can be solve by these two options
    // 1. Public method uses std::optional
    // 2. Create private method which takes TilePatternDescriptorPtr and gets 
    //    its pattern by taking into account whether destinantion has a value

    TilePatternPtr tilePattern;
    /*
    if(destination.has_value())
        tilePattern = a_->CalculateTilePattern(origin, destination.value(), constraints) + b_->CalculateTilePattern(origin, destination.value(), constraints);
    else
        tilePattern = a_->CalculateTilePattern(origin, constraints) + b_->CalculateTilePattern(origin, constraints);
        */
     
    return tilePattern;
}