#include <Utils/Vector2.h>
#include <AWC/AWCfwd.h>

#include <vector>
#include <unordered_map>
#include <memory>


class TilePatternDescriptor
{
public:
    TilePatternDescriptor(const std::vector<Vector2>& directions);
    TilePatternDescriptor(std::vector<Vector2> directions, const std::unordered_map<Vector2, std::vector<Vector2>>& excludedDirections);

    std::shared_ptr<TilePattern> CalculateTilePattern(Vector2 origin, int range);
    std::shared_ptr<TilePattern> CalculatePatternWithDestination(Vector2 origin, Vector2 destination);

    void SetExcludedDirections(Vector2 dir, std::vector<Vector2> excludedDirections);
    std::vector<Vector2> GetLockedDirections(Vector2 dir);

private:

    std::unordered_map<Vector2, std::vector<Vector2>> GenerateDefaultLockedDirections(const std::vector<Vector2>& directions);
    std::unordered_map<Vector2, std::vector<Vector2>> GenerateLockedDirectionsMap(const std::vector<Vector2>& directions, const std::unordered_map<Vector2, std::vector<Vector2>>& excludedDirections);
    std::vector<Vector2> GenerateLockedDirections(const std::vector<Vector2>& directions, const std::vector<Vector2>& excludedDirections);

    std::vector<std::weak_ptr<TileNode>> DiscoverNeighbours(const Map& map, TileGraph& mg, int x, int y, const std::vector<Vector2>& directions);

    std::vector<Vector2> directions_;
    std::unordered_map<Vector2, std::vector<Vector2>> lockedDirectionsMap_;
};