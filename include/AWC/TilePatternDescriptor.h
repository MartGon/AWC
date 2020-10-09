#include <Utils/Vector2.h>
#include <AWC/AWCfwd.h>

#include <vector>
#include <unordered_map>
#include <memory>

class TilePatternDescriptor
{
public:

    static TilePatternDescriptor CreateTilePatternDescriptor(const std::vector<Vector2>& directions) {return TilePatternDescriptor{directions};};
    static TilePatternDescriptor CreateTilePatternDescriptorByLockedDirectionsMap(const std::vector<Vector2>& directions, const std::unordered_map<Vector2, std::vector<Vector2>>& lockedDirectionsMap);
    static TilePatternDescriptor CreateTilePatternDescriptorByExclusiveDirectionsMap(const std::vector<Vector2>& directions, const std::unordered_map<Vector2, std::vector<Vector2>>& exclusiveDirectionsMap);

    std::shared_ptr<TilePattern> CalculateTilePattern(Vector2 origin, int range);
    std::shared_ptr<TilePattern> CalculatePatternWithDestination(Vector2 origin, Vector2 destination);

    void SetExclusiveDirections(Vector2 dir, std::vector<Vector2> exclusiveDirections);
    void SetLockedDirections(Vector2 dir, std::vector<Vector2> lockedDirections);
    std::vector<Vector2> GetLockedDirections(Vector2 dir);

private:
    TilePatternDescriptor(const std::vector<Vector2>& directions);
    TilePatternDescriptor(const std::vector<Vector2>& directions, const std::unordered_map<Vector2, std::vector<Vector2>>& lockedDirectionsMap);

    static std::unordered_map<Vector2, std::vector<Vector2>> GenerateDefaultLockedDirectionsMap(const std::vector<Vector2>& directions);
    static std::unordered_map<Vector2, std::vector<Vector2>> GenerateLockedDirectionsMap(const std::vector<Vector2>& directions, const std::unordered_map<Vector2, std::vector<Vector2>>& exclusiveDirections);
    static std::vector<Vector2> GenerateLockedDirections(const std::vector<Vector2>& directions, const std::vector<Vector2>& exclusiveDirections);

    std::vector<std::weak_ptr<TileNode>> DiscoverNeighbours(const Map& map, TileGraph& mg, int x, int y, const std::vector<Vector2>& directions);

    std::vector<Vector2> directions_;
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap_;
};