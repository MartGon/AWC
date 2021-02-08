#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class Move : public OperationI
    {
    public:
        ~Move() override {};
        Move(uint mapIndex, Vector2 origin, Vector2 dest) : mapIndex_{mapIndex}, 
            origin_{origin}, dest_{dest}, OperationI{Type::MOVE} {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        uint mapIndex_;
        Vector2 origin_;
        Vector2 dest_;
    };
}