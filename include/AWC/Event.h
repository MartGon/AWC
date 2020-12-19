#pragma once

#include <AWC/GameEvent.h>

#include <functional>
#include <unordered_map>

namespace Event
{
    enum class Type
    {
        TURN_PASSED
    };

    union Union
    {
        TurnPassed turnPassed;
    };

    struct Event
    {
        Type type;
        Union event;
    };


    // Subject
    using EventListener = std::function<void(Event)>;
    class Subject
    {
    public:
        void Register(Type type, EventListener eventListener);
        void Notify(Event ev);
        void Notify(Event ev, Type type);

    private:
        std::unordered_map<Type, std::vector<EventListener>> eventListeners_;
    };
}