#include <AWC/Operation/Attack.h>
#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/StatMod.h>

#include <AWC/Game.h>

using namespace Operation;

Result Attack::Execute(Game& game, const Process::Process& p)
{
    Result result{SUCCESS};

    // TODO: Check if the unit can attack there

    auto& map = game.GetMap(origin_.mapIndex);
    attacker_ = map.GetUnit(origin_.pos);

    Process::Trigger::Trigger t{Process::Trigger::Type::OPERATION, p.id};

    // TODO: Get units inside attack (it can be an AoE)
    // TODO: Check if the weapon has enough ammo
    if(auto targetUnit = map.GetUnit(dest_.pos))
    {
        auto dmg = attacker_->GetDmgToUnit(weaponIndex_, targetUnit);
        OperationIPtr op{new TakeDmg(targetUnit, dmg, this)};
        game.Push(op, t, p.priority);
    }

    StatMod::Extra extra{StatMod::Ammo{weaponIndex_}};
    OperationIPtr ammoOp{new StatMod(attacker_, UnitNS::AMMO, -1, extra)};
    game.Push(ammoOp, t, p.priority);

    return result;
}