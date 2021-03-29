#pragma once

#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class TakeDmg : public OperationI
    {
    public:
        TakeDmg(UnitPtr victim, float attackDmg, OperationI* source) :  victim_{victim}, attackDmg_{attackDmg}, 
            source_{source}, OperationI{Type::TAKE_DMG} {};
        ~TakeDmg() override {};

        Result Execute(Game& state, Process::Info info) override;
    
        UnitPtr victim_;
        float attackDmg_;
        OperationI* source_;
    };
}