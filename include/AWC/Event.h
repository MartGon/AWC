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
    using Handler = std::function<void(Process, NotificationType)>;

    struct Listener
    {
        Operation::Type type;
        Handler handler;
    };

    class Subject
    {
    public:
        void Register(Listener listener);
        void Register(Operation::Type type, Handler eventListener);
        void Notify(Process p, NotificationType notType);
        void Notify(Process p, Operation::Type type, NotificationType notType);

    private:
        std::unordered_map<Operation::Type, std::vector<Handler>> eventListeners_;
    };
}