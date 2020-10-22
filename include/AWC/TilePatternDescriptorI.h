#include <AWC/AWCfwd.h>
#include <Utils/Vector2.h>

#include <memory>
#include <optional>

using TilePatternPtr = std::shared_ptr<TilePattern>;

class TilePatternDescriptorI
{
public:
    ~TilePatternDescriptorI() {}

    TilePatternPtr CalculateTilePattern(Vector2 origin, const TilePatternConstraints& constraints);
    TilePatternPtr CalculateTilePattern(Vector2 origin, Vector2 destination, const TilePatternConstraints& constraints);

private:
    virtual TilePatternPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, const TilePatternConstraints& constraints) = 0;
};