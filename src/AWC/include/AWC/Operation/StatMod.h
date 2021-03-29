#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class StatMod : public OperationI
    {    
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

        StatMod(UnitPtr unit, UnitNS::StatType type, int amount, Extra extra = Extra{}) : 
            unit_{unit}, type_{type}, amount_{amount}, extra_{extra}, OperationI{Type::STAT_MOD} {};
        ~StatMod() override {};

        Result Execute(Game& state, Process::Info info) override;
    
        UnitPtr unit_;
        UnitNS::StatType type_;
        int amount_;
        Extra extra_;
    };
}