#pragma once
#include "AWCfwd.h"
#include "Utils.h"

#include <memory>
#include <unordered_map>

using CommandPtr = std::shared_ptr<Command>;
using CostTableIPtr = std::shared_ptr<CostTableI>;
using CostTablePtr = std::shared_ptr<CostTable>;
using Directions = std::vector<Vector2>;
using DirectionsTable = std::unordered_map<Vector2, Directions>;
using MovementDescPtr = std::shared_ptr<MovementDesc>;
using MovementDescTypePtr = std::shared_ptr<MovementDescType>;
using TileNodePtr = std::weak_ptr<TileNode>;
using TilePatternDescIPtr = std::shared_ptr<TilePatternDescI>;
using TilePatternDescPtr = std::shared_ptr<TilePatternDesc>;
using TilePatternIPtr = std::shared_ptr<TilePatternI>;
using TilePtr = std::shared_ptr<Tile>;
using UnitPtr = std::shared_ptr<Unit>;
using WeaponPtr = std::shared_ptr<Weapon>;
using WeaponTypePtr = std::shared_ptr<WeaponType>;
using OperationIPtr = std::shared_ptr<Operation::OperationI>;
using OpeartionIWPtr = std::weak_ptr<Operation::OperationI>;