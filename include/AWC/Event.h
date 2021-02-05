#pragma once

#include <AWC/AWCusing.h>
#include <functional>
#include <unordered_map>

namespace Event
{
    // Subject
    using Listener = std::function<void(OperationIPtr)>;
    class Subject
    {
    public:
        void Register(Operation::Type type, Listener eventListener);
        void Notify(OperationIPtr op);
        void Notify(OperationIPtr op, Operation::Type type);

    private:
        std::unordered_map<Operation::Type, std::vector<Listener>> eventListeners_;
    };
}