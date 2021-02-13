#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    class UpdateFlag : public OperationI
    {
    friend class Factory;

    public:
        ~UpdateFlag() override {};
        UpdateFlag(unsigned int id, UnitPtr unit, UnitNS::Flag flag, bool set) : unit_{unit}, 
            flag_{flag}, set_{set}, OperationI{id, Type::UPDATE_FLAG} {};

        Result Execute(Game& game) override;
    
        UnitPtr unit_;
        Vector2 unitPos_;
        UnitNS::Flag flag_;
        bool set_;
    };
}