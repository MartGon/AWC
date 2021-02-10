#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Process.h>
#include <AWC/Entity.h>

#include <functional>
#include <unordered_map>
#include <variant>
#include <queue>


namespace Event
{   
    namespace Notification
    {
        enum class Type
        {
            PRE,
            POST
        };

        struct Notification
        {  
            Notification(Type type, Process process) : 
                type{type}, process{process} {};

            Type type;
            Process process;
        };
    }

    using HandlerCallback = std::function<void(Notification::Notification, Entity::Entity, Game&)>;

    struct Handler
    {
        Handler(Operation::Type type, HandlerCallback callback) : type{type}, callback{callback} {};

        Operation::Type type;
        HandlerCallback callback;
    };

    struct Listener
    {
        Listener(Entity::Entity entity, Handler handler) :
            entity{entity}, handler{handler} {};
        Listener(Operation::Type type, Entity::Entity entity, HandlerCallback handler) :
            entity{entity}, handler{type, handler} {};

        Entity::Entity entity;
        Handler handler;
    };

    class Subject
    {
    public:
        void Register(Listener listener);
        void Register(Entity::Entity entity, Operation::Type type, HandlerCallback eventListener);
        void Register(Operation::Type type, HandlerCallback callback);
        void Notify(Process p, Notification::Type notType, Game& game);
        void Notify(Process p, Operation::Type type, Notification::Type notType, Game& game);
        void Notify(Notification::Notification notification, Game& game);

    private:
        std::unordered_map<Operation::Type, std::queue<Listener>> eventListeners_;
    };
}