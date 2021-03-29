#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/UpdateFlag.h>

#include <AWC/Game.h>

using namespace Operation;

Result TakeDmg::Execute(Game& game, Process::Info info)
{
    Result result{SUCCESS};

    victim_->TakeDamage(attackDmg_);
    auto health = victim_->GetHealth();

    if(health <= 0)
    {
        OperationIPtr op {new UpdateFlag(victim_, UnitNS::DEAD, true)};
        game.Push(op, info);
    }

    return result;
}