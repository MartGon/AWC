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

    std::shared_ptr<Unit> CreateUnit(Player& owner);

    const std::string GetName() const;
    const uint GetId() const;

    std::vector<Event::Handler> GetHandlers() const;
    void AddHandler(Event::Handler listener);

private:

    std::vector<WeaponPtr> GetWeapons() const;

    std::string name_;
    uint id_;
    uint lastInstanceId_ = 0;

    MovementDecTypePtr moveType_;
    std::vector<WeaponTypePtr> weaponTypes_;
    std::vector<Event::Handler> listeners_;
};