#pragma once

#include "AWC/Operation/OperationI.h"


#include <AWC/Game.h>

#include "AWC/AWCusing.h"

namespace Operation
{
    class Attack : public OperationI
    {
    friend class Factory;

    public:
        Attack(Position origin, Position dest, uint weaponIndex) :
            origin_{origin}, dest_{dest}, weaponIndex_{weaponIndex}, OperationI{Type::ATTACK} {};
        ~Attack() override {};

        Result Execute(Game& state, uint8_t prio) override;
    
        UnitPtr attacker_;
        Position origin_;
        Position dest_;
        uint weaponIndex_;
    };
}