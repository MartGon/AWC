#include "AWC/Operation/OperationI.h"

namespace Operation
{
    enum class ComposedType
    {

    };

    class Composed : public OperationI
    {
    public:
        ~Composed() override {
            for(auto child : children)
                delete child;
        };
        Composed() : OperationI{Type::COMPOSED} {};

        void AddChild(OperationI* child) { children.push_back(child); };

        Result Execute(Game& state) override;

    private:
        std::vector<OperationI*> children;
    };
}