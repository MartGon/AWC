#pragma once

#include <Utils.h>

#include <AWC/AWCusing.h>
#include <AWC/Directions.h>
#include <AWC/Area/AreaDescI.h>
#include <AWC/Area/Node.h>

#include <unordered_map>
#include <memory>
#include <exception>
#include <optional>

class AreaDesc : public AreaDescI
{
public:

    static AreaDescPtr Create(const Directions& directions);
    static AreaDescPtr CreateByLocked(const Directions& directions, const DirectionsTable& lockedDirectionsTable);
    static AreaDescPtr CreateByExclusive(const Directions& directions, const DirectionsTable& exclusiveDirectionsTable);

    Directions GetOriginDirections() const;
    bool IsOriginDirection(Vector2 dir);
    void AddOriginDirection(Vector2 dir);
    void RemoveOriginDirection(Vector2 dir);

    Directions GetLockedDirections(Vector2 dir) const;
    void SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections);
    void SetLockedDirections(Vector2 dir, const Directions& lockedDirections);

private:

    static const Vector2 NULL_MOVEMENT;

    AreaDesc(const Directions& directions);
    AreaDesc(const Directions& directions, const DirectionsTable& lockedDirectionsTable);

    static DirectionsTable GenerateDefaultLockedDirectionsTable(const Directions& directions);
    static DirectionsTable PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections);
    static DirectionsTable GenerateLockedDirectionsTable(const Directions& directions, const DirectionsTable& exclusiveDirections);
    static Directions GenerateLockedDirections(const Directions& directions, const Directions& exclusiveDirections);

    AreaIPtr DoCalculateArea(const Map& map, Vector2 origin, std::optional<Vector2> destination, const AreaConstraints& constraints) override;

    Directions originDirections_;
    DirectionsTable lockedDirectionsTable_;
};