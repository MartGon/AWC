#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    class UpdateFlag : public OperationI
    {
    public:
        ~UpdateFlag() override {};
        UpdateFlag(UnitPtr unit, UnitNS::Flag flag, bool set) : unit_{unit}, 
            flag_{flag}, set_{set}, OperationI{Type::UPDATE_FLAG} {};

        Result Execute(Game& game, uint8_t prio) override;
    
        UnitPtr unit_;
        Vector2 unitPos_;
        UnitNS::Flag flag_;
        bool set_;
    };
}