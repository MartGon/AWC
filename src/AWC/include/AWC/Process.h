#pragma once
#include <AWC/AWCusing.h>

enum Priority
{
    PRIORITY_DEFAULT = 128
};

struct Process
{
    Process(unsigned int id, OperationIPtr op, uint8_t prio = PRIORITY_DEFAULT, bool announced = false) : id{id}, priority{prio}, op{op}, announced{false} {};

    unsigned int id;
    uint8_t priority;
    OperationIPtr op;
    bool announced;
};