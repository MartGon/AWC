#pragma once

#include <AWC/AWCusing.h>
#include <AWC/TilePattern/TilePatternDescI.h>
#include <AWC/TilePattern/TilePatternDesc.h>

template<typename T>
class TilePatternDescComp : public TilePatternDescI
{
public:
    TilePatternDescComp(TilePatternDescIPtr a, TilePatternDescIPtr b) : a_{a}, b_{b} {};

private:

    TilePatternIPtr GetTilePattern(TilePatternDescIPtr tpdip, Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints)
    {
        TilePatternIPtr res;
        if(destination.has_value())
            res = tpdip->CalculateTilePattern(map, origin, destination.value(), constraints);
        else
            res = tpdip->CalculateTilePattern(map, origin, constraints);
        
        return res;
    }

    virtual TilePatternIPtr DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const TilePatternConstraints& constraints)
    {
        TilePatternIPtr tilePattern;
        auto tilePatternA = GetTilePattern(a_, origin, destination, map, constraints);
        auto tilePatternB = GetTilePattern(b_, origin, destination, map,  constraints);
        tilePattern = std::shared_ptr<T>(new T{tilePatternA, tilePatternB});
        
        return tilePattern;
    }

    TilePatternDescIPtr a_;
    TilePatternDescIPtr b_;
};

using TilePatternDescUnion = TilePatternDescComp<TilePatternUnion>;
using TilePatternDescDiff = TilePatternDescComp<TilePatternDiff>;
using TilePatternDescIntersect = TilePatternDescComp<TilePatternIntersect>;