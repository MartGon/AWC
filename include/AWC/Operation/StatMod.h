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
            uint weaponIndex;
        };

        union Extra
        {
            Null null;
            Ammo ammo;
        };

        ~StatMod() override {};
        
        StatMod(UnitPtr unit, UnitNS::StatType type, int amount, Extra extra = Extra{}) : 
            unit_{unit}, type_{type}, amount_{amount}, OperationI{Type::STAT_MOD} {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        UnitNS::StatType type_;
        int amount_;
        Extra extra_;
    };
}