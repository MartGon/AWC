#pragma once

#include <AWC/Operation/OperationI.h>


namespace Operation
{
    class AntiOperation : public OperationI
    {
    public:
        AntiOperation(unsigned int targetId) : targetId_{targetId}, OperationI{Type::ANTI_OPERATION} {};
        ~AntiOperation() override {} ;

        Result Execute(Game& state, const Process::Process& p) override;

        unsigned int targetId_;
    };
}