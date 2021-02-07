#pragma once

#include <AWC/AWCusing.h>
#include <functional>
#include <unordered_map>


namespace Event
{
    enum class NotificationType
    {
        PRE,
        POST
    };

    // Subject
    using Listener = std::function<void(Process, NotificationType)>;
    class Subject
    {
    public:
        void Register(Operation::Type type, Listener eventListener);
        void Notify(Process p, NotificationType notType);
        void Notify(Process p, Operation::Type type, NotificationType notType);

    private:
        std::unordered_map<Operation::Type, std::vector<Listener>> eventListeners_;
    };
}