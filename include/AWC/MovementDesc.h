#pragma once

#include <AWC/AWCfwd.h>

class MovementDesc
{
friend class MovementDescType;

public:
    unsigned int currentGas;
    const MovementDescType& moveType;

private:
    MovementDesc(const MovementDescType& moveType);
};