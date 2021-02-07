#include <AWC/Event.h>

#include <Utils/STLUtils.h>
#include <AWC/Operation/Operation.h>
#include <AWC/Process.h>

using namespace Event;
using namespace Operation;

void Subject::Register(Listener listener)
{
    Subject::Register(listener.type, listener.handler);
}

void Subject::Register(Operation::Type type, Handler eventListener)
{
    if(!UnorderedMapUtils::Contains(eventListeners_, type))
    {
        eventListeners_[type] = std::vector<Handler>{};
    }

    auto& typeListeners = eventListeners_.at(type);
    typeListeners.push_back(eventListener);
}

void Subject::Notify(Process p, Operation::Type type, NotificationType notType)
{
    if(UnorderedMapUtils::Contains(eventListeners_, type))
    {
        std::vector<Handler> typeListeners = eventListeners_.at(type);
        for(auto listener : typeListeners)
        {
            listener(p, notType);
        }
    }
}

void Subject::Notify(Process p, NotificationType notType)
{
    Type type = p.op->GetType();
    Notify(p, type, notType);
}