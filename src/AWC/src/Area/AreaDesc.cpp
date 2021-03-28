#include <AWC/Area/Area.h>
#include <AWC/Area/AreaDesc.h>
#include <AWC/Area/AreaConstraints.h>
#include <AWC/CostTable.h>
#include <AWC/Map.h>
#include <AWC/Area/Graph.h>
#include <AWC/Exception.h>
#include <AWC/Pathfinding.h>

#include <Utils.h>

#include <limits>
#include <queue>
#include <functional>
#include <optional>

// Public

// Factory methods
AreaDescPtr AreaDesc::Create(const Directions& directions)
{
    return AreaDescPtr{new AreaDesc{directions}};
}

AreaDescPtr AreaDesc::CreateByLocked(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable)
{
    return std::shared_ptr<AreaDesc>{
        new AreaDesc{directions, PrepareLockedDirectionsTable(directions, lockedDirectionsTable)
        }};
}

AreaDescPtr  AreaDesc::CreateByExclusive(const Directions& directions, 
    const DirectionsTable& exclusiveDirectionsTable)
{
    return std::shared_ptr<AreaDesc>{
        new AreaDesc{directions, GenerateLockedDirectionsTable(directions, exclusiveDirectionsTable)
        }};
}

// Constructors
AreaDesc::AreaDesc(const Directions& directions) : 
    originDirections_{directions}, lockedDirectionsTable_{GenerateDefaultLockedDirectionsTable(directions)}
{

}

AreaDesc::AreaDesc(const Directions& directions, 
    const DirectionsTable& lockedDirectionsTable) : 
    originDirections_{directions}, lockedDirectionsTable_{lockedDirectionsTable}
{

}

// Methods

Directions AreaDesc::GetOriginDirections() const
{
    return originDirections_;
}

bool AreaDesc::IsOriginDirection(Vector2 dir)
{
    return VectorUtils::IsInside(originDirections_, dir);
}

void AreaDesc::AddOriginDirection(Vector2 dir)
{
    if(!IsOriginDirection(dir))
        originDirections_.push_back(dir);
    else
        throw AWC::AlreadyExistingIndexException("AreaDesc::AddOriginDirection", dir);
}

void AreaDesc::RemoveOriginDirection(Vector2 dir)
{
    if(IsOriginDirection(dir))
    {
        VectorUtils::RemoveByValue(originDirections_, dir);
        lockedDirectionsTable_.erase(dir);
    }
    else
        throw AWC::NoExistingIndexException("AreaDesc::RemoveOriginDirection", dir);

}

Directions AreaDesc::GetLockedDirections(Vector2 dir) const
{
    return lockedDirectionsTable_.at(dir);
}

void AreaDesc::SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections)
{
    lockedDirectionsTable_[dir] = GenerateLockedDirections(originDirections_, exclusiveDirections);
}

void AreaDesc::SetLockedDirections(Vector2 dir, const Directions& lockedDirections)
{
    lockedDirectionsTable_[dir] = lockedDirections;
}

// Private

const Vector2 AreaDesc::NULL_MOVEMENT = Vector2{0, 0};

DirectionsTable AreaDesc::GenerateDefaultLockedDirectionsTable(const Directions& directions)
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

DirectionsTable AreaDesc::PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections)
{
    auto finalLockedDirectionsTable = lockedDirections;
    finalLockedDirectionsTable.insert({NULL_MOVEMENT, directions});
    return finalLockedDirectionsTable;
}

DirectionsTable AreaDesc::GenerateLockedDirectionsTable(const Directions& directions, 
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

Directions AreaDesc::GenerateLockedDirections(const Directions& directions, 
    const Directions& exclusiveDirections)
{
    Directions lockedDirections = directions;
    for(const auto exclusiveDir : exclusiveDirections)
        VectorUtils::RemoveByValue(lockedDirections, exclusiveDir);

    return lockedDirections;
}

AreaIPtr AreaDesc::DoCalculateArea(const Map& map, Vector2 origin, 
    std::optional<Vector2> destination, const AreaConstraints& constraints)
{
    Pathfinding::Params params{map, *this, constraints, destination};
    auto tg = Pathfinding::Dijkstra(origin, params);

    // Create Area
    auto tp = AreaIPtr(new Area{origin, tg, params.constraints.range});

    return tp;
}