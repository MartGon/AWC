#pragma once

#include <Utils/Vector2.h>

#include <AWC/AWCusing.h>
#include <AWC/Directions.h>
#include <AWC/TilePattern/TilePatternDescI.h>
#include <AWC/TilePattern/TileNode.h>

#include <unordered_map>
#include <memory>
#include <exception>
#include <optional>

class TilePatternDesc : public TilePatternDescI
{
public:

    static TilePatternDescPtr Create(const Directions& directions) 
        {return TilePatternDescPtr{new TilePatternDesc{directions}};}
    static TilePatternDescPtr CreateByLocked(const Directions& directions, const DirectionsTable& lockedDirectionsTable);
    static TilePatternDescPtr CreateByExclusive(const Directions& directions, const DirectionsTable& exclusiveDirectionsTable);

    Directions GetOriginDirections();
    bool IsOriginDirection(Vector2 dir);
    void AddOriginDirection(Vector2 dir);
    void RemoveOriginDirection(Vector2 dir);

    Directions GetLockedDirections(Vector2 dir);
    void SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections);
    void SetLockedDirections(Vector2 dir, const Directions& lockedDirections);

private:

    static const Vector2 NULL_MOVEMENT;

    TilePatternDesc(const Directions& directions);
    TilePatternDesc(const Directions& directions, const DirectionsTable& lockedDirectionsTable);

    static DirectionsTable GenerateDefaultLockedDirectionsTable(const Directions& directions);
    static DirectionsTable PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections);
    static DirectionsTable GenerateLockedDirectionsTable(const Directions& directions, const DirectionsTable& exclusiveDirections);
    static Directions GenerateLockedDirections(const Directions& directions, const Directions& exclusiveDirections);

    TilePatternIPtr DoCalculateTilePattern(const Map& map, Vector2 origin, std::optional<Vector2> destination, const TilePatternConstraints& constraints) override;

    Directions GetDiscoverDirections(TileNodePtr tileNode, const Map& map);
    Vector2 GetMovementToOrigin(TileNodePtr tileNode);
    Directions GetValidDirections(TileNodePtr tileNode, Directions directions, const Map& map);
    unsigned int GetTileCost(const Map& map, const TilePatternConstraints& tpc, Vector2 pos);

    Directions originDirections_;
    DirectionsTable lockedDirectionsTable_;
};