#pragma once
#include <AWC/AWCfwd.h>
#include <Utils/Vector2.h>

#include <memory>
#include <unordered_map>

using CostTableIPtr = std::shared_ptr<CostTableI>;
using Directions = std::vector<Vector2>;
using DirectionsTable = std::unordered_map<Vector2, Directions>;
using TileNodePtr = std::weak_ptr<TileNode>;
using TilePatternDescIPtr = std::shared_ptr<TilePatternDescI>;
using TilePatternDescPtr = std::shared_ptr<TilePatternDesc>;
using TilePatternIPtr = std::shared_ptr<TilePatternI>;
using TilePtr = std::shared_ptr<Tile>;
using UnitPtr = std::shared_ptr<Unit>;
