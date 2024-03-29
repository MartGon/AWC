#pragma once
#include "AWCfwd.h"
#include "Utils.h"

#include <memory>
#include <unordered_map>

using CommandPtr = std::shared_ptr<Command>;
using Directions = std::vector<Vector2>;
using DirectionsTable = std::unordered_map<Vector2, Directions>;
using MovementDescPtr = std::shared_ptr<MovementDesc>;
using MovementDescTypePtr = std::shared_ptr<MovementDescType>;
using NodePtr = std::weak_ptr<Node>;
using AreaDescIPtr = std::shared_ptr<AreaDescI>;
using AreaDescPtr = std::shared_ptr<AreaDesc>;
using AreaIPtr = std::shared_ptr<AreaI>;
using TilePtr = std::shared_ptr<Tile>;
using UnitPtr = std::shared_ptr<Unit>;
using WeaponPtr = std::shared_ptr<Weapon>;
using WeaponTypePtr = std::shared_ptr<WeaponType>;
using OperationIPtr = std::shared_ptr<Operation::OperationI>;
using OpeartionIWPtr = std::weak_ptr<Operation::OperationI>;