#include <AWC/AWCfwd.h>
#include <AWC/TilePatternDescriptorI.h>

#include <memory>

using TilePatternConstraintsPtr = std::shared_ptr<TilePatternConstraints>;
using TilePatternDescriptorIPtr = std::shared_ptr<TilePatternDescriptorI>;

class TPDFixedRange : public TilePatternDescriptorI
{
public:
    TPDFixedRange(TilePatternDescriptorIPtr child, unsigned int maxRange, unsigned int minRange = 0);

private:
    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, 
        const Map& map, const TilePatternConstraints& constraints) override;

    TilePatternDescriptorIPtr child_;
    unsigned int minRange_;
    unsigned int maxRange_;
};