#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Area/AreaDescI.h>
#include <AWC/Area/AreaDesc.h>

template<typename T>
class AreaDescComp : public AreaDescI
{
public:
    AreaDescComp(AreaDescIPtr a, AreaDescIPtr b) : a_{a}, b_{b} {};

private:

    AreaIPtr GetArea(AreaDescIPtr tpdip, Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const AreaConstraints& constraints)
    {
        AreaIPtr res;
        if(destination.has_value())
            res = tpdip->CalculateArea(map, origin, destination.value(), constraints);
        else
            res = tpdip->CalculateArea(map, origin, constraints);
        
        return res;
    }

    virtual AreaIPtr DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, 
        const AreaConstraints& constraints)
    {
        AreaIPtr area;
        auto areaA = GetArea(a_, origin, destination, map, constraints);
        auto areaB = GetArea(b_, origin, destination, map,  constraints);
        area = std::shared_ptr<T>(new T{areaA, areaB});
        
        return area;
    }

    AreaDescIPtr a_;
    AreaDescIPtr b_;
};

using AreaDescUnion = AreaDescComp<AreaUnion>;
using AreaDescDiff = AreaDescComp<AreaDiff>;
using AreaDescIntersect = AreaDescComp<AreaIntersect>;