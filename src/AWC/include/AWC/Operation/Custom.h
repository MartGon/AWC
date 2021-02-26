#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    class Custom : public OperationI
    {    
    public:
        Custom(std::function<void(Game&)> func) : func_{func}, OperationI{Type::CUSTOM} {};
        ~Custom() override {};
        Result Execute(Game& state, uint8_t prio) override { func_(state); return Result{SUCCESS}; }

    private:
        std::function<void(Game&)> func_;
    };
}