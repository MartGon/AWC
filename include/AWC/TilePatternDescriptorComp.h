#pragma once

#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescriptorI.h>
#include <AWC/TilePatternDescriptor.h>

using TilePatternDescriptorIPtr = std::shared_ptr<TilePatternDescriptorI>;

template<typename T>
class TilePatternDescriptorComp : public TilePatternDescriptorI
{
public:
    TilePatternDescriptorComp(TilePatternDescriptorIPtr a, TilePatternDescriptorIPtr b) : a_{a}, b_{b} {};

private:

    TilePatternIPtr GetTilePattern(TilePatternDescriptorIPtr tpdip, Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints)
        {
            TilePatternIPtr res;
            if(destination.has_value())
                res = tpdip->CalculateTilePattern(origin, destination.value(), map, constraints);
            else
                res = tpdip->CalculateTilePattern(origin, map, constraints);
            
            return res;
        }
    virtual TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints)
    {
        TilePatternIPtr tilePattern;
        auto tilePatternA = GetTilePattern(a_, origin, destination, map, constraints);
        auto tilePatternB = GetTilePattern(b_, origin, destination, map,  constraints);
        tilePattern = std::shared_ptr<T>(new T{tilePatternA, tilePatternB});
        
        return tilePattern;
    }

    TilePatternDescriptorIPtr a_;
    TilePatternDescriptorIPtr b_;
};

using TilePatternDescriptorUnion = TilePatternDescriptorComp<TilePatternUnion>;
using TilePatternDescriptorDiff = TilePatternDescriptorComp<TilePatternDiff>;