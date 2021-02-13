#pragma once

#include "AWC/Operation/OperationI.h"
#include <AWC/Operation/Factory.h>

#include <AWC/Game.h>

#include "AWC/AWCusing.h"

namespace Operation
{
    class Attack : public OperationI
    {
    friend class Factory;

    public:
        ~Attack() override {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        Position origin_;
        Position dest_;
        uint weaponIndex_;

    private:
        Attack(unsigned int id, Position origin, Position dest, uint weaponIndex) :
            origin_{origin}, dest_{dest}, weaponIndex_{weaponIndex}, OperationI{id, Type::ATTACK} {};
    };
}