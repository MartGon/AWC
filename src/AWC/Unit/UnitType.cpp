#include <AWC/Unit/UnitType.h>
#include <AWC/Unit/Unit.h>

#include <AWC/Unit/MovementDescType.h>
#include <AWC/Unit/WeaponType.h>

UnitType::UnitType(uint id, const std::string& name, MovementDecTypePtr moveType, std::vector<WeaponTypePtr> weaponTypes) 
    : id_{id}, name_{name}, moveType_{moveType}, weaponTypes_{weaponTypes}
{

}

std::shared_ptr<Unit> UnitType::CreateUnit(Player& owner) const
{
    return UnitPtr( new Unit{*this, moveType_->CreateMovementDesc(), GetWeapons(), owner});
}

const std::string UnitType::GetName() const
{
    return name_;
}

const uint UnitType::GetId() const
{
    return id_;
}

// Private

std::vector<WeaponPtr> UnitType::GetWeapons() const
{
    auto weaponAmount = weaponTypes_.size();
    std::vector <WeaponPtr> weapons{weaponAmount};
    for(uint i = 0; i < weaponAmount; i++)
        weapons[i] = weaponTypes_[i]->CreateWeapon();

    return weapons;
}