#include <AWC/AWCusing.h>

enum Priority
{
    PRIORITY_DEFAULT = 128
};

struct Process
{
    Process(OperationIPtr op, uint8_t prio = PRIORITY_DEFAULT, bool announced = false) : priority{prio}, op{op}, announced{false} {};

    uint8_t priority;
    OperationIPtr op;
    bool announced;
};