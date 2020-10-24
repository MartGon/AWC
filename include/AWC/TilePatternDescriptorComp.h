#pragma once

#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescriptorI.h>
#include <AWC/TilePatternDescriptor.h>

using TilePatternDescriptorIPtr = std::shared_ptr<TilePatternDescriptorI>;

class TilePatternDescriptorUnion : public TilePatternDescriptorI
{
public:
    TilePatternDescriptorUnion(TilePatternDescriptorIPtr a, TilePatternDescriptorIPtr b);

private:

    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, const Map& map, const TilePatternConstraints& constraints) override;

    TilePatternDescriptorIPtr a_;
    TilePatternDescriptorIPtr b_;
};