#pragma once

#include <AWC/AWCusing.h>
#include <memory>
#include <string>

#include <AWC/Event.h>


class Unit;

class UnitType
{
public:
    UnitType(uint id, const std::string& name, MovementDecTypePtr moveType, std::vector<WeaponTypePtr> weaponTypes);

    std::shared_ptr<Unit> CreateUnit(Player& owner) const;

    const std::string GetName() const;
    const uint GetId() const;

    std::vector<Event::Listener> GetListeners() const;
    void AddListener(Event::Listener listener);

private:

    std::vector<WeaponPtr> GetWeapons() const;

    std::string name_;
    uint id_;

    MovementDecTypePtr moveType_;
    std::vector<WeaponTypePtr> weaponTypes_;
    std::vector<Event::Listener> listeners_;
};