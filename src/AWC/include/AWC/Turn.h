#pragma once

class Turn
{
public:
    Turn();
    Turn(unsigned int playerIndex);

    unsigned int playerIndex;

    // CommandHistory that were executed
};