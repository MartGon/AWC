#include <AWC/Unit.h>
#include <AWC/UnitType.h>
#include <AWC/UnitMovement.h>

#include <AWC/TilePatternConstraints.h>

#include <AWC/MovementDescType.h>
#include <AWC/MovementDesc.h>
#include <AWC/Weapon.h>
#include <AWC/WeaponType.h>
#include <AWC/TilePatternDesc.h>

#include <AWC/AWCException.h>

Unit::Unit(const UnitType& unitType, const MovementDescPtr movementDesc, const std::vector<WeaponPtr> weapons) : unitType_{unitType}, weapons_{weapons}, moveDesc_{movementDesc}
{

}

const std::string Unit::GetName() const
{
    return unitType_.GetName();
}

const uint Unit::GetId() const
{
    return unitType_.GetId();
}

// Movement

UnitMovement Unit::CalculateMovement(const Map& map, Vector2 origin)
{
    auto moveDesc = moveDesc_->GetMovePattern();
    auto moveConstraints = GetMoveConstraints();
    auto tp = moveDesc->CalculateTilePattern(map, origin, moveConstraints);
    return UnitMovement{tp};
}

// Attack

bool Unit::CanAttack(UnitPtr unit)
{
    for(uint weaponId = 0; weaponId < GetWeaponCount(); weaponId++)
        if(CanAttackWith(unit, weaponId))
            return true;
    
    return false;
}

bool Unit::CanAttackWith(UnitPtr unit, uint weaponId)
{
    bool canAttack = false;
    if(IsWeaponIdValid(weaponId))
    {
        auto weapon = weapons_[weaponId];
        canAttack = weapon->weaponType.attackTable.CanAttack(unit->GetId());
    }
    else    
        throw std::out_of_range{"Invalid weaponId: " + std::to_string(weaponId)};

    return canAttack;
}

uint Unit::GetWeaponCount()
{
    return weapons_.size();
}

// private

    // Movement

TilePatternConstraints Unit::GetMoveConstraints() const
{
    // Note: Some of these could be modified by buffs/powerups
    auto tileCost = moveDesc_->GetTileCostTable();
    auto unitCost = moveDesc_->GetUnitCostTable();
    auto range = moveDesc_->GetRange();

    TilePatternConstraints tpc{tileCost, unitCost, range};
    return tpc;
}

    // Attack

bool Unit::IsWeaponIdValid(uint weaponId)
{
    return weaponId < weapons_.size();
}