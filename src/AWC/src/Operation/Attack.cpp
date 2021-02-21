#include <AWC/Operation/Attack.h>
#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/StatMod.h>

#include <AWC/Game.h>

using namespace Operation;

Result Attack::Execute(Game& game, uint8_t prio)
{
    Result result{SUCCESS};

    // TODO: Check if the unit can attack there

    auto& map = game.GetMap(origin_.mapIndex);
    attacker_ = map.GetUnit(origin_.pos);

    // TODO: Get units inside attack (it can be an AoE)
    // TODO: Check if the weapon has enough ammo

    auto& opFactory = game.GetOperationFactory();
    if(auto targetUnit = map.GetUnit(dest_.pos))
    {
        auto dmg = attacker_->GetDmgToUnit(weaponIndex_, targetUnit);
        OperationIPtr op = opFactory.CreateTakeDmg(targetUnit, dmg, this);
        game.Push(op, prio);
    }

    StatMod::Extra extra{StatMod::Ammo{weaponIndex_}};
    OperationIPtr ammoOp = opFactory.CreateStatMod(attacker_, UnitNS::AMMO, -1, extra);
    game.Push(ammoOp, prio);

    return result;
}