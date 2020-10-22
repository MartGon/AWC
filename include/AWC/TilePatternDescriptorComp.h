#pragma once

#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescriptorI.h>
#include <AWC/TilePatternDescriptor.h>

class TilePatternDescriptorUnion : public TilePatternDescriptorI
{
public:
    TilePatternDescriptorUnion(TilePatternDescriptorPtr a, TilePatternDescriptorPtr b);

private:

    TilePatternPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, const TilePatternConstraints& constraints) override;

    TilePatternDescriptorPtr a_;
    TilePatternDescriptorPtr b_;
};