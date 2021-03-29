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

    struct Info
    {
        Info(unsigned int id, uint8_t prio = PRIORITY_DEFAULT, 
            Trigger::Trigger trigger = Trigger::Trigger{Trigger::Type::NONE, 0}) : id{id}, priority{prio}, announced{false}, trigger{trigger} {};

        unsigned int id;
        uint8_t priority;
        bool announced;
        Trigger::Trigger trigger;
    };

    struct Process
    {
        Process(unsigned int id, OperationIPtr op, uint8_t prio = PRIORITY_DEFAULT, 
            Trigger::Trigger trigger = Trigger::Trigger{Trigger::Type::NONE, 0}) : info{id, prio, trigger}, op{op} {};

        
        Info info;
        OperationIPtr op;
    };
}