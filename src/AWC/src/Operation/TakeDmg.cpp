#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/UpdateFlag.h>

#include <AWC/Game.h>

using namespace Operation;

Result TakeDmg::Execute(Game& game, uint8_t prio)
{
    Result result{SUCCESS};

    victim_->TakeDamage(attackDmg_);
    auto health = victim_->GetHealth();

    if(health <= 0)
    {
        OperationIPtr op {new UpdateFlag(victim_, UnitNS::DEAD, true)};
        game.Push(op, prio);
    }

    return result;
}