#pragma once

#include <AWC/Operation/OperationI.h>


namespace Operation
{
    class AntiOperation : public OperationI
    {
    friend class Factory;

    public:
        ~AntiOperation() override {} ;

        Result Execute(Game& state, uint8_t prio) override;

        unsigned int targetId_;
    private:
        AntiOperation(unsigned int id, unsigned int targetId) : targetId_{targetId}, OperationI{id, Type::ANTI_OPERATION} {};
    };
}