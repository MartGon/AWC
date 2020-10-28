
#include <AWC/AWCfwd.h>

#include <Utils/Vector2.h>

#include <memory>
#include <vector>

using TilePatternDescIPtr = std::shared_ptr<TilePatternDescI>;
using TilePatternIPtr = std::shared_ptr<TilePatternI>;
using CostTableIPtr = std::shared_ptr<CostTableI>;

class UnitMovementDesc
{
public:
    UnitMovementDesc(CostTableIPtr tileCost, CostTableIPtr unitCost, unsigned int minRange, unsigned int maxRange);

    UnitMovement CalculateUnitMovement(const Map& map, Vector2 origin, Vector2 dest) const;
    UnitMovement CalculateUnitMovement(const Map& map, Vector2 origin) const;

private:

    TilePatternConstraints GetMoveConstraints() const;

    TilePatternDescIPtr tilePatternDesc_;
    CostTableIPtr tileCost_;
    CostTableIPtr unitCost_;
    unsigned int minRange_;
    unsigned int maxRange_;

    // Game properties
    // uint gasLeft, etc.;
};

class UnitMovement
{
public:
    UnitMovement(TilePatternIPtr tp);

    bool CanMove(Vector2 dest) const;
    std::vector<Vector2> GetPathTo(Vector2 dest) const;
    std::vector<Vector2> GetTilesPosInReach() const;

private:
    TilePatternIPtr tp_;
};