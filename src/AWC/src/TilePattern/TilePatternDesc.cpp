#include <AWC/TilePattern/TilePattern.h>
#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/TilePattern/TilePatternConstraints.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/TilePattern/TileGraph.h>
#include <AWC/AWCException.h>
#include <AWC/Pathfinding.h>

#include <Utils.h>

#include <limits>
#include <queue>
#include <functional>
#include <optional>

// Public

// Factory methods
TilePatternDescPtr TilePatternDesc::CreateByLocked(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable)
{
    return std::shared_ptr<TilePatternDesc>{
        new TilePatternDesc{directions, PrepareLockedDirectionsTable(directions, lockedDirectionsTable)
        }};
}

TilePatternDescPtr  TilePatternDesc::CreateByExclusive(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    return std::shared_ptr<TilePatternDesc>{
        new TilePatternDesc{directions, GenerateLockedDirectionsTable(directions, exclusiveDirectionsTable)
        }};
}

// Constructors
TilePatternDesc::TilePatternDesc(const Directions& directions) : 
    originDirections_{directions}, lockedDirectionsTable_{GenerateDefaultLockedDirectionsTable(directions)}
{

}

TilePatternDesc::TilePatternDesc(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable) : 
    originDirections_{directions}, lockedDirectionsTable_{lockedDirectionsTable}
{

}

// Methods

Directions TilePatternDesc::GetOriginDirections() const
{
    return originDirections_;
}

bool TilePatternDesc::IsOriginDirection(Vector2 dir)
{
    return VectorUtils::IsInside(originDirections_, dir);
}

void TilePatternDesc::AddOriginDirection(Vector2 dir)
{
    if(!IsOriginDirection(dir))
        originDirections_.push_back(dir);
    else
        throw AWCAlreadyExistingIndexException("TilePatternDesc::AddOriginDirection", dir);
}

void TilePatternDesc::RemoveOriginDirection(Vector2 dir)
{
    if(IsOriginDirection(dir))
    {
        VectorUtils::RemoveByValue(originDirections_, dir);
        lockedDirectionsTable_.erase(dir);
    }
    else
        throw AWCNoExistingIndexException("TilePatternDesc::RemoveOriginDirection", dir);

}

Directions TilePatternDesc::GetLockedDirections(Vector2 dir) const
{
    return lockedDirectionsTable_.at(dir);
}

void TilePatternDesc::SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections)
{
    lockedDirectionsTable_[dir] = GenerateLockedDirections(originDirections_, exclusiveDirections);
}

void TilePatternDesc::SetLockedDirections(Vector2 dir, const Directions& lockedDirections)
{
    lockedDirectionsTable_[dir] = lockedDirections;
}

// Private

const Vector2 TilePatternDesc::NULL_MOVEMENT = Vector2{0, 0};

DirectionsTable TilePatternDesc::GenerateDefaultLockedDirectionsTable(const Directions& directions)
{
    DirectionsTable lockedDirectionsTable;
    lockedDirectionsTable.insert({NULL_MOVEMENT, directions});

    for(const auto dir : directions)
    {
        // The intention is to remove loops
        Vector2 neg = -dir;
        Directions lockedDirections = GenerateLockedDirections(directions, {neg});
        lockedDirectionsTable.insert({dir, lockedDirections});
    }

    return lockedDirectionsTable;
}

DirectionsTable TilePatternDesc::PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections)
{
    auto finalLockedDirectionsTable = lockedDirections;
    finalLockedDirectionsTable.insert({NULL_MOVEMENT, directions});
    return finalLockedDirectionsTable;
}

DirectionsTable TilePatternDesc::GenerateLockedDirectionsTable(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    DirectionsTable lockedDirectionsTable;
    lockedDirectionsTable.insert({NULL_MOVEMENT, directions});

    for(const auto pair : exclusiveDirectionsTable)
    {
        auto dir = pair.first;
        auto exclusiveDirections = pair.second;

        auto lockedDirectionsForDir = GenerateLockedDirections(directions, exclusiveDirections);
        lockedDirectionsTable.insert({dir, lockedDirectionsForDir});
    }

    return lockedDirectionsTable;
}

Directions TilePatternDesc::GenerateLockedDirections(const Directions& directions, 
    const Directions& exclusiveDirections)
{
    Directions lockedDirections = directions;
    for(const auto exclusiveDir : exclusiveDirections)
        VectorUtils::RemoveByValue(lockedDirections, exclusiveDir);

    return lockedDirections;
}

TilePatternIPtr TilePatternDesc::DoCalculateTilePattern(const Map& map, Vector2 origin, 
    std::optional<Vector2> destination, const TilePatternConstraints& constraints)
{
    Pathfinding::Params params{map, *this, constraints, destination};
    auto tg = Pathfinding::Dijkstra(origin, params);

    // Create TilePattern
    auto tp = TilePatternIPtr(new TilePattern{origin, tg, params.constraints.range});

    return tp;
}