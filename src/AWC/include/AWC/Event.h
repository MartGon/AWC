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
            Notification(Type type, Process::Process process) : 
                type{type}, process{process} {};
            Notification(Type type, Process::Process process, Operation::Result res) :
                type{type}, process{process}, res{res} {}

            Type type;
            Process::Process process;
            std::optional<Operation::Result> res;
        };
    }

    class CallbackI
    {
    public:
        virtual void Call(const Notification::Notification, Entity::GUID, Game&) = 0;
    };

    using CallbackFunction = std::function<void(const Notification::Notification, Entity::GUID, Game&)>;

    class Callback : public CallbackI
    {
    public:
        Callback(CallbackFunction callback) : callback{callback} {};

        void Call(const Notification::Notification notification, Entity::GUID e, Game& g) override
        {
            callback(notification, e, g);
        }

        CallbackFunction callback;
    };

    class Handler
    {
    public:
        Handler(unsigned int opType, std::shared_ptr<CallbackI> handler, Notification::Type type = Notification::Type::ANY) :
            handler{handler}, notificationType{type}, opType{opType} {};
        Handler(unsigned int opType, CallbackFunction handlerCB, Notification::Type type = Notification::Type::ANY) :
            Handler{opType, std::make_shared<Callback>(handlerCB), type} {};

        void Call(const Notification::Notification notification, Entity::GUID e, Game& g)
        {
            handler->Call(notification, e, g);
        }

        bool Handles(Notification::Type type);

        std::shared_ptr<CallbackI> GetHandler() const
        {
            return handler;
        }

        Notification::Type notificationType;
        unsigned int opType;

    private:
        std::shared_ptr<CallbackI> handler;
    };

    struct Listener
    {
        Entity::GUID entity;
        Handler handler;
    };

    class Subject
    {
    public:
        void Register(Listener listener);
        void Register(Entity::GUID entity, Handler handler);

        template <typename ...Args>
        void Register(Entity::GUID entity, unsigned int opType, Args&& ...args)
        {
            Register(entity, Handler{ opType, args...});
        }
        Entity::GUID Register(unsigned int opType, CallbackFunction callback, Notification::Type notType = Notification::Type::ANY);
        Entity::GUID Register(Handler handler);

        void Unregister(Entity::GUID entity, unsigned int optype);
        void Unregister(Entity::GUID entity);

        void Notify(Process::Process p, Notification::Type notType, Game& game);
        void Notify(Process::Process p, Operation::Type type, Notification::Type notType, Game& game);
        void Notify(Notification::Notification notification, Game& game);

    private:

        void RemoveListeners(std::vector<Listener>& listeners, Entity::GUID ent);
        
        // TODO: Change to id system, then allow to register for any or a mask of operation types -> Change operation types to powers of 2
        // It says unsigned int, but it actually is Operation::Type
        std::unordered_map<unsigned int, std::vector<Listener>> eventListeners_;

        unsigned int lastId = 0;
    };
}