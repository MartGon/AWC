#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class Move : public OperationI
    {    
    public:
        Move(uint mapIndex, Vector2 origin, Vector2 dest) : mapIndex_{mapIndex}, 
            origin_{origin}, dest_{dest}, OperationI{Type::MOVE} {};
        ~Move() override {};

        Result Execute(Game& state, uint8_t prio) override;
    
        UnitPtr unit_;
        uint mapIndex_;
        Vector2 origin_;
        Vector2 dest_;
    };
}