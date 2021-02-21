#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Process.h>
#include <AWC/Entity.h>

#include <functional>
#include <unordered_map>
#include <variant>

namespace Event
{   
    namespace Notification
    {
        enum class Type
        {
            NONE,
            PRE,
            POST,
            ANY
        };

        struct Notification
        {  
            Notification(Type type, Process process) : 
                type{type}, process{process} {};

            Type type;
            Process process;
        };
    }

    using HandlerCallback = std::function<void(Notification::Notification, Entity::GUID, Game&)>;

    struct Handler
    {
        Handler(Operation::Type type, HandlerCallback callback, Notification::Type notificationType = Notification::Type::ANY) : 
            type{type}, callback{callback}, notificationType{notificationType} {};

        bool ListensTo(Notification::Type type);

        Operation::Type type;
        HandlerCallback callback;
        Notification::Type notificationType;
    };

    struct Listener
    {
        Listener(Entity::GUID entity, Handler handler) :
            entity{entity}, handler{handler} {};
        Listener(Entity::GUID entity, Operation::Type opType, HandlerCallback handler, Notification::Type type = Notification::Type::ANY) :
            entity{entity}, handler{opType, handler, type} {};

        Entity::GUID entity;
        Handler handler;
    };

    class Subject
    {
    public:
        void Register(Listener listener);
        void Register(Entity::GUID entity, Operation::Type type, HandlerCallback eventListener, Notification::Type notType = Notification::Type::ANY);
        void Register(Operation::Type opType, HandlerCallback callback, Notification::Type notType = Notification::Type::ANY);

        void Unregister(Entity::GUID entity, Operation::Type type);
        void Unregister(Entity::GUID entity);

        void Notify(Process p, Notification::Type notType, Game& game);
        void Notify(Process p, Operation::Type type, Notification::Type notType, Game& game);
        void Notify(Notification::Notification notification, Game& game);

    private:

        void RemoveListeners(std::vector<Listener>& listeners, Entity::GUID ent);

        std::unordered_map<Operation::Type, std::vector<Listener>> eventListeners_;
    };
}