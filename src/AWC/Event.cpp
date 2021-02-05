#include <AWC/Event.h>

#include <Utils/STLUtils.h>
#include <AWC/Operation/Operation.h>

using namespace Event;
using namespace Operation;

void Subject::Register(Operation::Type type, Listener eventListener)
{
    if(!UnorderedMapUtils::Contains(eventListeners_, type))
    {
        eventListeners_[type] = std::vector<Listener>{};
    }

    auto& typeListeners = eventListeners_.at(type);
    typeListeners.push_back(eventListener);
}

void Subject::Notify(OperationIPtr op, Operation::Type type)
{
    if(UnorderedMapUtils::Contains(eventListeners_, type))
    {
        std::vector<Listener> typeListeners = eventListeners_.at(type);
        for(auto listener : typeListeners)
        {
            listener(op);
        }
    }
}

void Subject::Notify(OperationIPtr op)
{
    Type type = op->GetType();
    Notify(op, type);
}