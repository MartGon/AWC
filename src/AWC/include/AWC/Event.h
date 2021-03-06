#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Process.h>
#include <AWC/Entity.h>
#include <AWC/Operation/OperationI.h>

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
            ERROR,
            ANY
        };

        struct Notification
        {  
            Notification(Type type, Process process) : 
                type{type}, process{process} {};
            Notification(Type type, Process process, Operation::Result res) :
                type{type}, process{process}, res{res} {}

            Type type;
            Process process;
            std::optional<Operation::Result> res;
        };
    }

    using HandlerCallback = std::function<void(const Notification::Notification, Entity::GUID, Game&)>;

    struct Handler
    {
        Handler(unsigned int type, HandlerCallback callback, Notification::Type notificationType = Notification::Type::ANY) : 
            type{type}, callback{callback}, notificationType{notificationType} {};

        bool ListensTo(Notification::Type type);

        unsigned int type;
        HandlerCallback callback;
        Notification::Type notificationType;
    };

    struct Listener
    {
        Listener(Entity::GUID entity, Handler handler) :
            entity{entity}, handler{handler} {};
        Listener(Entity::GUID entity, unsigned int opType, HandlerCallback handler, Notification::Type type = Notification::Type::ANY) :
            entity{entity}, handler{opType, handler, type} {};

        Entity::GUID entity;
        Handler handler;
    };

    class Subject
    {
    public:
        void Register(Listener listener);
        void Register(Entity::GUID entity, unsigned int type, HandlerCallback callback, Notification::Type notType = Notification::Type::ANY);
        void Register(unsigned int opType, HandlerCallback callback, Notification::Type notType = Notification::Type::ANY);

        void Unregister(Entity::GUID entity, unsigned int type);
        void Unregister(Entity::GUID entity);

        void Notify(Process p, Notification::Type notType, Game& game);
        void Notify(Process p, Operation::Type type, Notification::Type notType, Game& game);
        void Notify(Notification::Notification notification, Game& game);

    private:

        void RemoveListeners(std::vector<Listener>& listeners, Entity::GUID ent);

        std::unordered_map<unsigned int, std::vector<Listener>> eventListeners_;
    };
}