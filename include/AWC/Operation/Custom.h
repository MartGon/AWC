#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    class Custom : public OperationI
    {
    friend class Factory;
    
    public:
        ~Custom() override {};
        Result Execute(Game& state) override { func_(state); return Result{SUCCESS}; }

    private:
        Custom(unsigned int id, std::function<void(Game&)> func) : func_{func}, OperationI{id, Type::CUSTOM} {};
        std::function<void(Game&)> func_;
    };
}