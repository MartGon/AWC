#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    enum class ComposedType
    {

    };

    class Composed : public OperationI
    { 
    public:
        Composed() : OperationI{Type::COMPOSED} {};
        ~Composed() override {
            for(auto child : children)
                delete child;
        };

        void AddChild(OperationI* child) { children.push_back(child); };

        Result Execute(Game& state, const Process::Process& p) override;

    private:
        std::vector<OperationI*> children;
    };
}