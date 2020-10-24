#pragma once

#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>
#include <AWC/Directions.h>
#include <AWC/TilePatternDescriptorI.h>
#include <AWC/TileNode.h>

#include <unordered_map>
#include <memory>
#include <exception>
#include <optional>

using TilePatternDescriptorPtr = std::shared_ptr<TilePatternDescriptor>;
using TilePatternPtr = std::shared_ptr<TilePattern>;
using DirectionsTable = std::unordered_map<Vector2, Directions>;

class TilePatternDescriptor : public TilePatternDescriptorI
{
public:

    static TilePatternDescriptorPtr Create(const Directions& directions) 
        {return TilePatternDescriptorPtr{new TilePatternDescriptor{directions}};}
    static TilePatternDescriptorPtr CreateByLocked(const Directions& directions, const DirectionsTable& lockedDirectionsTable);
    static TilePatternDescriptorPtr CreateByExclusive(const Directions& directions, const DirectionsTable& exclusiveDirectionsTable);

    Directions GetOriginDirections();
    bool IsOriginDirection(Vector2 dir);
    void AddOriginDirection(Vector2 dir);
    void RemoveOriginDirection(Vector2 dir);

    Directions GetLockedDirections(Vector2 dir);
    void SetExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections);
    void SetLockedDirections(Vector2 dir, const Directions& lockedDirections);

private:

    static const Vector2 NULL_MOVEMENT;

    TilePatternDescriptor(const Directions& directions);
    TilePatternDescriptor(const Directions& directions, const DirectionsTable& lockedDirectionsTable);

    static DirectionsTable GenerateDefaultLockedDirectionsTable(const Directions& directions);
    static DirectionsTable PrepareLockedDirectionsTable(const Directions& directions, const DirectionsTable& lockedDirections);
    static DirectionsTable GenerateLockedDirectionsTable(const Directions& directions, const DirectionsTable& exclusiveDirections);
    static Directions GenerateLockedDirections(const Directions& directions, const Directions& exclusiveDirections);

    TilePatternIPtr DoCalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, const Map& map, const TilePatternConstraints& constraints) override;

    Directions GetDiscoverDirections(TileNodePtr tileNode, const Map& map);
    Vector2 GetMovementToOrigin(TileNodePtr tileNode);
    Directions GetValidDirections(TileNodePtr tileNode, Directions directions, const Map& map);
    unsigned int GetTileCost(const Map& map, const TilePatternConstraints& tpc, Vector2 pos);

    Directions originDirections_;
    DirectionsTable lockedDirectionsTable_;
};