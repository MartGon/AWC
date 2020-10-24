#include <AWC/TilePatternDescriptorComp.h>
#include <AWC/TilePatternComp.h>

// Union

TilePatternDescriptorUnion::TilePatternDescriptorUnion(TilePatternDescriptorPtr a, TilePatternDescriptorPtr b) : a_{a}, b_{b}
{

}

TilePatternIPtr TilePatternDescriptorUnion::DoCalculateTilePattern(Vector2 origin, 
    std::optional<Vector2> destination, const TilePatternConstraints& constraints)
{
    // There is redundancy here
    // It can be solved by these two options
    // 1. Public method uses std::optional
    // 2. Create private method which takes TilePatternDescriptorPtr and gets 
    //    its pattern by taking into account whether destinantion has a value

    TilePatternIPtr tilePattern;
    if(destination.has_value())
    {
        auto tilePatternA = a_->CalculateTilePattern(origin, destination.value(), constraints);
        auto tilePatternB = b_->CalculateTilePattern(origin, destination.value(), constraints);
        tilePattern = std::make_shared<TilePatternUnion>(tilePatternA, tilePatternB);
    }
    else
    {
        auto tilePatternA = a_->CalculateTilePattern(origin, constraints);
        auto tilePatternB = b_->CalculateTilePattern(origin, constraints);
        tilePattern = std::make_shared<TilePatternUnion>(tilePatternA, tilePatternB);
    }
        
     
    return tilePattern;
}