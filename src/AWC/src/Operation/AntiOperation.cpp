#include <AWC/Operation/AntiOperation.h>

#include <AWC/Game.h>

using namespace Operation;

Result AntiOperation::Execute(Game& game, uint8_t prio)
{
    Result res{SUCCESS};

    game.RemoveProcess(targetId_);

    // TODO: Check if the opQueue size changed
    // or RemoveProcess could return a bool or something

    return res;
}