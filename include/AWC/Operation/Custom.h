#include "AWC/Operation/OperationI.h"


namespace Operation
{
    class Custom : public OperationI
    {
    public:
        ~Custom() override {};
        Custom(std::function<void(Game&)> func) : func_{func}, OperationI{Type::CUSTOM} {};
        Result Execute(Game& state) override { func_(state); return Result{SUCCESS}; }
    private:
        std::function<void(Game&)> func_;
    };
}