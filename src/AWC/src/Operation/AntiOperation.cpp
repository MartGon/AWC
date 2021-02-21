#include <AWC/Operation/AntiOperation.h>

#include <AWC/Game.h>

using namespace Operation;

Result AntiOperation::Execute(Game& game, uint8_t prio)
{
    Result res{SUCCESS};

    game.RemoveOperation(targetId_);

    // TODO: Check if the opQueue size changed
    // or RemoveOperation could return a bool or something

    return res;
}