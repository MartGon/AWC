#include <AWC/Event.h>

#include <Utils/STLUtils.h>
#include <AWC/Operation/Operation.h>
#include <AWC/Process.h>

using namespace Event;
using namespace Operation;

bool Listener::ListensTo(Notification::Type type)
{
    return notificationType == Notification::Type::ANY || notificationType == type;
}

void Subject::Register(Listener listener)
{
    auto type = listener.handler.type;
    if(!UnorderedMapUtils::Contains(eventListeners_, type))
    {
        eventListeners_[type] = std::vector<Listener>{};
    }
    
    auto& typeListeners = eventListeners_.at(type);
    typeListeners.push_back(listener);
}

void Subject::Register(Entity::Entity entity, Operation::Type type, HandlerCallback eventListener, Notification::Type notType)
{
    Listener listener{entity, type, eventListener, notType};
    Subject::Register(listener);
}

void Subject::Register(Operation::Type type, HandlerCallback cb, Notification::Type notType)
{
    Event::Handler handler{type, cb};
    Event::Listener listener{Entity::NIL, handler, notType};
    Register(listener);
}

void Subject::Unregister(Entity::Entity entity, Operation::Type type)
{
    if(UnorderedMapUtils::Contains(eventListeners_, type))
    {
        auto& typeListeners = eventListeners_.at(type);
        std::vector<unsigned int> indexToRemove;

        for(unsigned int i = 0; i < typeListeners.size(); i++)
        {
            auto listener = typeListeners.at(i);
            if(listener.entity == entity)
                indexToRemove.push_back(i);
        }

        for(auto index : indexToRemove)
            VectorUtils::RemoveByIndex(typeListeners, index);
    }
}

void Subject::Notify(Notification::Notification notification, Game& game)
{
    auto opType = notification.process.op->GetType();
    if(UnorderedMapUtils::Contains(eventListeners_, opType))
    {
        auto typeListeners = eventListeners_.at(opType);

        // TODO: Maybe sort listeners by GUID.

        for(auto listener : typeListeners)
        {
            auto callback = listener.handler.callback;
            if(listener.ListensTo(notification.type))
                callback(notification, listener.entity, game);
        }
    }
}

void Subject::Notify(Process p, Operation::Type type, Notification::Type notType, Game& game)
{
    Notification::Notification noti{notType, p};
    Notify(noti, game);
}

void Subject::Notify(Process p, Notification::Type notType, Game& game)
{
    Notification::Notification noti{notType, p};
    Notify(noti, game);
}