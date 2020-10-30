#include <AWC/UnitType.h>
#include <AWC/Unit.h>

#include <AWC/MovementDescType.h>
#include <AWC/WeaponType.h>

UnitType::UnitType(uint id, const std::string& name, MovementDecTypePtr moveType, WeaponTypePtr weaponType) 
    : id_{id}, name_{name}, moveType_{moveType}, weaponType_{weaponType}
{

}

std::shared_ptr<Unit> UnitType::CreateUnit() const
{
    return std::make_shared<Unit>(Unit{*this, moveType_->CreateMovementDesc(), weaponType_->CreateWeapon()});
}

const std::string UnitType::GetName() const
{
    return name_;
}

const uint UnitType::GetId() const
{
    return id_;
}