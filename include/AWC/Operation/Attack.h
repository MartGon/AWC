#pragma once

#include "AWC/Operation/OperationI.h"

#include <AWC/Game.h>

#include "AWC/AWCusing.h"

namespace Operation
{
    class Attack : public OperationI
    {
    public:
        ~Attack() override {};
        Attack(Position origin, Position dest, uint weaponIndex) :
            origin_{origin}, dest_{dest}, weaponIndex_{weaponIndex}, OperationI{Type::ATTACK} {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        Position origin_;
        Position dest_;
        uint weaponIndex_;
    };
}