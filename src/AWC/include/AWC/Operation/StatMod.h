#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class StatMod : public OperationI
    {
    friend class Factory;
    
    public:

        struct Null
        {
        };

        struct Ammo
        {
            Ammo(uint weaponIndex) : weaponIndex{weaponIndex} {};

            uint weaponIndex;
        };

        union Extra
        {
            Extra() {};
            Extra(Ammo ammo) : ammo{ammo} {};

            Null null;
            Ammo ammo;
        };

        ~StatMod() override {};

        Result Execute(Game& state, uint8_t prio) override;
    
        UnitPtr unit_;
        UnitNS::StatType type_;
        int amount_;
        Extra extra_;

    private:
        StatMod(unsigned int id, UnitPtr unit, UnitNS::StatType type, int amount, Extra extra = Extra{}) : 
            unit_{unit}, type_{type}, amount_{amount}, extra_{extra}, OperationI{id, Type::STAT_MOD} {};
    };
}