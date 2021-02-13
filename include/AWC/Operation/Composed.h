#pragma once

#include "AWC/Operation/OperationI.h"

namespace Operation
{
    enum class ComposedType
    {

    };

    class Composed : public OperationI
    {
    friend class Factory;
    
    public:
        ~Composed() override {
            for(auto child : children)
                delete child;
        };

        void AddChild(OperationI* child) { children.push_back(child); };

        Result Execute(Game& state) override;

    private:
        Composed(unsigned int id) : OperationI{id, Type::COMPOSED} {};

        std::vector<OperationI*> children;
    };
}