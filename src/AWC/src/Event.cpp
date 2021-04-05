#include <AWC/Event.h>

#include <Utils.h>
#include <AWC/Operation/Operation.h>
#include <AWC/Process.h>

using namespace Event;
using namespace Operation;

bool Handler::Handles(Notification::Type type)
{
    return notificationType == Notification::Type::ANY || notificationType == type;
}

void Subject::Register(Listener listener)
{
    auto opType = listener.handler.opType;

    if(!UnorderedMapUtils::Contains(eventListeners_, opType))
    {
        eventListeners_[opType] = std::vector<Listener>{};
    }
    
    auto& typeListeners = eventListeners_.at(opType);
    typeListeners.push_back(listener);
}

void Subject::Register(Entity::GUID entity, Handler handler)
{
    Register(Listener{entity, handler});
}

Entity::GUID Subject::Register(unsigned int type, CallbackFunction cb, Notification::Type notType)
{
    return Register(Event::Handler{type, cb, notType});
}

Entity::GUID Subject::Register(Event::Handler handler)
{
    Entity::GUID entity{Entity::Type::HANDLER, 0, lastId++};
    Register(entity, handler);

    return entity;
}

void Subject::Unregister(Entity::GUID ent)
{
    for(auto& pair : eventListeners_)
    {
        auto& typeListeners = pair.second;
        RemoveListeners(typeListeners, ent);
    }

    return;
}

void Subject::Unregister(Entity::GUID entity, unsigned int type)
{
    if(UnorderedMapUtils::Contains(eventListeners_, type))
    {
        auto& typeListeners = eventListeners_.at(type);
        RemoveListeners(typeListeners, entity);
    }

    return;
}

void Subject::RemoveListeners(std::vector<Listener>& typeListeners, Entity::GUID ent)
{
    std::vector<unsigned int> indexToRemove;

    for(unsigned int i = 0; i < typeListeners.size(); i++)
    {
        auto listener = typeListeners.at(i);
        if(listener.entity == ent)
            indexToRemove.push_back(i);
    }

    for(auto index : indexToRemove)
        VectorUtils::RemoveByIndex(typeListeners, index);

    return;
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
            if(listener.handler.Handles(notification.type))
                listener.handler.Call(notification, listener.entity, game);
        }
    }
}

void Subject::Notify(Process::Process p, Operation::Type type, Notification::Type notType, Game& game)
{
    Notification::Notification noti{notType, p};
    Notify(noti, game);
}

void Subject::Notify(Process::Process p, Notification::Type notType, Game& game)
{
    Notification::Notification noti{notType, p};
    Notify(noti, game);
}