#include <Utils/Vector2.h>

#include <AWC/AWCfwd.h>
#include <AWC/Directions.h>
#include <AWC/TileNode.h>

#include <unordered_map>
#include <memory>
#include <exception>
#include <optional>

using TilePatternDescriptorPtr = std::shared_ptr<TilePatternDescriptor>;
using TilePatternPtr = std::shared_ptr<TilePattern>;
using DirectionsTable = std::unordered_map<Vector2, Directions>;

class TilePatternDescriptor
{
public:

    static TilePatternDescriptorPtr Create(const Directions& directions) 
        {return TilePatternDescriptorPtr{new TilePatternDescriptor{directions}};}
    static TilePatternDescriptorPtr CreateByLocked(const Directions& directions, const DirectionsTable& lockedDirectionsTable);
    static TilePatternDescriptorPtr CreateByExclusive(const Directions& directions, const DirectionsTable& exclusiveDirectionsTable);

    TilePatternPtr CalculateTilePattern(Vector2 origin, TilePatternConstraints constraints);
    TilePatternPtr CalculateTilePattern(Vector2 origin, std::optional<Vector2> destination, TilePatternConstraints constraints);

    Directions GetDirections();

    bool IsDirection(Vector2 dir);
    void AddDirection(Vector2 dir);
    void RemoveDirection(Vector2 dir);

    Directions GetLockedDirections(Vector2 dir);

    void TableExclusiveDirections(Vector2 dir, const Directions& exclusiveDirections);
    void TableLockedDirections(Vector2 dir, const Directions& lockedDirections);

private:
    TilePatternDescriptor(const Directions& directions);
    TilePatternDescriptor(const Directions& directions, const DirectionsTable& lockedDirectionsTable);

    static DirectionsTable GenerateDefaultLockedDirectionsTable(const Directions& directions);
    static DirectionsTable GenerateLockedDirectionsTable(const Directions& directions, const DirectionsTable& exclusiveDirections);
    static Directions GenerateLockedDirections(const Directions& directions, const Directions& exclusiveDirections);

    Directions GetDiscoverDirections(TileNodePtr tileNode);

    Directions directions_;
    DirectionsTable lockedDirectionsTable_;
};

class TilePatternDescriptorException : public std::exception
{
public:
    TilePatternDescriptorException(const std::string& msg, Vector2 pos);
    const char* what() const noexcept;

    Vector2 pos;
private:
    std::string msg_;
};

class TilePatternDescriptorNoExistingDirection : public TilePatternDescriptorException
{
public:
    TilePatternDescriptorNoExistingDirection(Vector2 pos) : TilePatternDescriptorException(std::string("Neighbour at pos ") + pos + " does not exist", pos) {};
};

class TilePatternDescriptorAlreadyExistingDirection : public TilePatternDescriptorException
{
public:
    TilePatternDescriptorAlreadyExistingDirection(Vector2 pos) : TilePatternDescriptorException(std::string("Neighbour at pos ") + pos + " already exists", pos) {};
};