#include <AWC/Event.h>

#include <Utils/STLUtils.h>

using namespace Event;

void Subject::Register(Type type, EventListener eventListener)
{
    if(!UnorderedMapUtils::Contains(eventListeners_, type))
    {
        eventListeners_[type] = std::vector<EventListener>{};
    }

    auto& typeListeners = eventListeners_.at(type);
    typeListeners.push_back(eventListener);
}

void Subject::Notify(Event ev, Type type)
{
    if(UnorderedMapUtils::Contains(eventListeners_, type))
    {
        std::vector<EventListener> typeListeners = eventListeners_.at(type);
        for(auto listener : typeListeners)
        {
            listener(ev);
        }
    }
}

void Subject::Notify(Event ev)
{
    Type type = ev.type;
    Notify(ev, type);
}