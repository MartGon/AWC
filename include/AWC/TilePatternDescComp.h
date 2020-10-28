#pragma once

#include <AWC/AWCusing.h>
#include <AWC/TilePatternDescI.h>
#include <AWC/TilePatternDesc.h>

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

    TilePatternDescIPtr a_;
    TilePatternDescIPtr b_;
};

using TilePatternDescUnion = TilePatternDescComp<TilePatternUnion>;
using TilePatternDescDiff = TilePatternDescComp<TilePatternDiff>;
using TilePatternDescIntersect = TilePatternDescComp<TilePatternIntersect>;