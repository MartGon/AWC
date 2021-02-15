#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class TakeDmg : public OperationI
    {
    friend class Factory;

    public:
        ~TakeDmg() override {};

        Result Execute(Game& state, uint8_t prio) override;
    
        UnitPtr victim_;
        float attackDmg_;
        OperationI* source_;
    private:
        TakeDmg(unsigned int id, UnitPtr victim, float attackDmg, OperationI* source) :  victim_{victim}, attackDmg_{attackDmg}, 
            source_{source}, OperationI{id, Type::TAKE_DMG} {};
    };
}