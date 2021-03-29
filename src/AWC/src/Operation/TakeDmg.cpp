#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/UpdateFlag.h>

#include <AWC/Game.h>

using namespace Operation;

Result TakeDmg::Execute(Game& game, const Process::Process& p)
{
    Result result{SUCCESS};

    victim_->TakeDamage(attackDmg_);
    auto health = victim_->GetHealth();

    if(health <= 0)
    {
        OperationIPtr op {new UpdateFlag(victim_, UnitNS::DEAD, true)};
        Process::Trigger::Trigger t{Process::Trigger::Type::OPERATION, p.id};
        game.Push(op, t, p.priority);
    }

    return result;
}