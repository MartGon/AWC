#pragma once
#include <AWC/AWCusing.h>

enum Priority
{
    PRIORITY_DEFAULT = 128
};

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

struct Process
{
    Process(unsigned int id, OperationIPtr op, uint8_t prio = PRIORITY_DEFAULT, 
        Trigger::Trigger trigger = Trigger::Trigger{Trigger::Type::NONE, 0}) : id{id}, priority{prio}, op{op}, announced{false}, trigger{trigger} {};

    unsigned int id;
    uint8_t priority;
    bool announced;
    Trigger::Trigger trigger;

    OperationIPtr op;
};