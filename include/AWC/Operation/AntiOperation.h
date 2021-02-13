#pragma once

#include <AWC/Operation/OperationI.h>


namespace Operation
{
    class AntiOperation : public OperationI
    {
    friend class Factory;

    public:
        ~AntiOperation() override {} ;

        Result Execute(Game& state) override;

    private:
        AntiOperation(unsigned int id, unsigned int targetId) : targetId_{targetId}, OperationI{id, Type::ANTI_OPERATION} {};

        unsigned int targetId_;
    };
}