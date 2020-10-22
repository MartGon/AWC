#include <AWC/AWCfwd.h>
#include <AWC/TilePatternI.h>
#include <AWC/TilePattern.h>

#include <memory>

using TilePatternIPtr = std::shared_ptr<TilePatternI>;

class TilePatternUnion : public TilePatternI
{
friend class TilePatternDescriptorUnion;

public:
    TilePatternUnion(TilePatternIPtr a, TilePatternIPtr b);

    unsigned int GetTileCost(Vector2 dest) const override;
    bool IsTileInPattern(Vector2 dest) const override;

    // TODO: This may become a problem for AttackPatterns
    // Could still return the path but only with nodes with cost > minRange;
    std::vector<Vector2> GetPathToTile(Vector2 dest) const override;
    std::vector<Vector2> GetTilesPosInPattern() const override;
    Vector2 GetOrigin() const override;

private:

    TilePatternIPtr a_;
    TilePatternIPtr b_;
};