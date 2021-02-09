#include <AWC/Unit/Unit.h>
#include <AWC/Unit/UnitType.h>
#include <AWC/Unit/UnitMovement.h>
#include <AWC/Unit/UnitAttack.h>

#include <AWC/TilePattern/TilePatternConstraints.h>

#include <AWC/Unit/MovementDescType.h>
#include <AWC/Unit/MovementDesc.h>
#include <AWC/Unit/Weapon.h>
#include <AWC/Unit/WeaponType.h>
#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/TilePattern/TilePattern.h>
#include <AWC/CostTable.h>
#include <AWC/Player.h>
#include <AWC/Turn.h>

#include <AWC/AWCException.h>

Unit::Unit(uint id, const UnitType& unitType, const MovementDescPtr movementDesc, const std::vector<WeaponPtr> weapons, Player& owner) 
    : id_{id}, unitType_{unitType}, weapons_{weapons}, moveDesc_{movementDesc}, owner_{owner}, flags{UnitNS::Flag::NONE}
{

}

const std::string Unit::GetName() const
{
    return unitType_.GetName();
}

const uint Unit::GetTypeId() const
{
    return unitType_.GetId();
}

const Player& Unit::GetOwner() const
{
    return owner_;
}

UnitNS::GUID Unit::GetGUID() const
{
    return UnitNS::GUID{id_, GetTypeId()};
}

// Movement

UnitMovement Unit::CalculateMovement(const Map& map, Vector2 origin)
{
    auto moveDesc = moveDesc_->GetMovePattern();
    auto moveConstraints = GetMoveConstraints();
    auto tp = moveDesc->CalculateTilePattern(map, origin, moveConstraints);
    return UnitMovement{tp};
}

void Unit::Move(unsigned int moveCost)
{
    //TODO: Could check if that moveCost is possible. Check current gas and maxRange;
    moveDesc_->Move(moveCost);

    SetFlag(UnitNS::Flag::MOVED);
}

uint Unit::GetCurrentGas() const
{
    return moveDesc_->GetCurrentGas();
}

bool Unit::CanMove() const
{
    return !HasFlag(UnitNS::Flag::MOVED);
}

// Attack

UnitAttack Unit::CalculateAttack(unsigned int weaponId, const Map& map, Vector2 origin)
{
    UnitAttack unitAttack{TilePatternIPtr{nullptr}};
    if(IsWeaponIdValid(weaponId))
    {
        auto weapon = weapons_[weaponId];
        auto tpd = weapon->GetAttackPattern();
        auto attackConstraints = GetAttackConstraints(weaponId);
        auto tp = tpd->CalculateTilePattern(map, origin, attackConstraints);
        unitAttack = UnitAttack{tp};
    }
    else
        ThrowInvalidWeaponIdException(weaponId);
    
    return unitAttack;
}

bool Unit::CanAttack() const
{
    return !HasFlag(UnitNS::Flag::ATTACKED);
}

bool Unit::CanAttack(UnitPtr unit) const
{
    for(uint weaponId = 0; weaponId < GetWeaponCount(); weaponId++)
        if(CanAttackUnitWith(unit, weaponId))
            return true;
    
    return false;
}

bool Unit::CanAttackUnitWith(UnitPtr unit, uint weaponId) const
{
    bool canAttack = false;
    if(IsWeaponIdValid(weaponId))
    {
        auto weapon = weapons_[weaponId];
        canAttack = weapon->HasEnoughAmmo() && weapon->CanAttackUnit(unit->GetTypeId());
    }
    else    
        ThrowInvalidWeaponIdException(weaponId);

    return canAttack;
}

uint Unit::GetWeaponCount() const
{
    return weapons_.size();
}

float Unit::GetDmgToUnit(unsigned int weaponId, UnitPtr unit) const
{
    // This can be modified by dmg buffs
    float dmg = 0.0f;
    if(IsWeaponIdValid(weaponId))
    {
        auto weapon = weapons_[weaponId];
        dmg = weapon->GetBaseDamageTo(unit->GetTypeId());
    }
    else
        ThrowInvalidWeaponIdException(weaponId);

    return dmg;
}


void Unit::UseWeapon(unsigned int weaponId)
{
    if(IsWeaponIdValid(weaponId))
    {
        auto weapon = weapons_[weaponId];
        weapon->Use();

        SetFlag(UnitNS::Flag::ATTACKED);
    }
    else
        ThrowInvalidWeaponIdException(weaponId);
}

uint Unit::GetWeaponAmmo(unsigned int weaponId) const
{
    uint ammo = 0;
    if(IsWeaponIdValid(weaponId))
        ammo = weapons_[weaponId]->GetCurrentAmmo();
    else
        ThrowInvalidWeaponIdException(weaponId);

    return ammo;
}

// Defense

float Unit::GetDmgTaken(float incomingDmg)
{
    // This can by modified by buffs
    return incomingDmg;
}

void Unit::TakeRawDamage(float incomingDmg)
{
    // Reduces health by exact amount. Ignoring buffs
    health -= incomingDmg;
}

void Unit::TakeDamage(float incomingRawDmg)
{
    // Uses defense mods to calculate final health reduction.
    float dmg = GetDmgTaken(incomingRawDmg);
    TakeRawDamage(dmg);
}

float Unit::GetHealth()
{
    return health;
}

bool Unit::IsDead()
{
    return HasFlag(UnitNS::Flag::DEAD);
}

// State

void Unit::OnPassTurn(Turn& turn)
{
    if(turn.playerIndex == owner_.GetId())
    {
        RemoveFlag(UnitNS::Flag::MOVED);
        RemoveFlag(UnitNS::Flag::ATTACKED);
    }
}

void Unit::SetFlag(UnitNS::Flag flag)
{
    flags |= flag;
}

void Unit::RemoveFlag(UnitNS::Flag flag)
{
    flags = flags & ~flag;
}

bool Unit::HasFlag(UnitNS::Flag flag) const
{
    return flags & flag;
}

// Events

void Unit::RegisterHandlers(Event::Subject& subject)
{   
    Entity::Entity entity{Entity::Type::UNIT, GetGUID()};
    auto handlers = unitType_.GetHandlers();
    for(auto handler : handlers)
    {
        Event::Listener listener{entity, handler};
        subject.Register(listener);
    }
}

// private

    // Movement

TilePatternConstraints Unit::GetMoveConstraints() const
{
    // Note: Some of these could be modified by buffs/powerups
    auto tileCost = moveDesc_->GetTileCostTable();
    auto unitCost = moveDesc_->GetUnitCostTable();
    auto range = moveDesc_->GetRange();
    auto teamId = owner_.GetTeamId();

    TilePatternConstraints tpc{tileCost, unitCost, teamId, range};
    return tpc;
}

    // Attack

TilePatternConstraints Unit::GetAttackConstraints(unsigned int weaponId) const
{
    auto weapon = weapons_[weaponId];

    CostTableIPtr fixedCost{ new FixedCostTable{1} };
    CostTableIPtr unitFixedCost{ new FixedCostTable{0} };

    // Range could be affected by mods;
    auto range = weapon->GetAttackRange();
    auto teamId = owner_.GetTeamId();

    return TilePatternConstraints{fixedCost, unitFixedCost, teamId, range};
}

bool Unit::IsWeaponIdValid(uint weaponId) const
{
    return weaponId < weapons_.size();
}

void Unit::ThrowInvalidWeaponIdException(uint weaponId) const
{
    throw std::out_of_range{"Invalid weaponId: " + std::to_string(weaponId)};
}