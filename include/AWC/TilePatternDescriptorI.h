#pragma once
#include <AWC/AWCfwd.h>
#include <Utils/Vector2.h>

#include <memory>
#include <optional>

using TilePatternIPtr = std::shared_ptr<TilePatternI>;

class TilePatternDescriptorI
{
public:
    virtual ~TilePatternDescriptorI() {}

    TilePatternIPtr CalculateTilePattern(Vector2 origin, const TilePatternConstraints& constraints);
    TilePatternIPtr CalculateTilePattern(Vector2 origin, Vector2 destination, const TilePatternConstraints& constraints);

private:
    virtual TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, const TilePatternConstraints& constraints) = 0;
};