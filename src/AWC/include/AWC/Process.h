#pragma once
#include <AWC/AWCusing.h>


namespace Process
{
    namespace Trigger
    {
        enum class Type : uint8_t{
            NONE,
            PLAYER,
            OPERATION
        };

        struct Trigger
        {
            Trigger(Type type, unsigned int id) : type{type}, id{id}
            {

            }

            Type type;
            unsigned int id;
        };
    }

    enum Priority
    {
        PRIORITY_DEFAULT = 128
    };

    struct Process
    {
        Process(unsigned int id, OperationIPtr op, Trigger::Trigger trigger, uint8_t prio = PRIORITY_DEFAULT) : 
            id{id}, priority{prio}, trigger{trigger}, op{op} {};

        unsigned int id;
        uint8_t priority;
        bool announced = false;
        Trigger::Trigger trigger;

        OperationIPtr op;
    };
}