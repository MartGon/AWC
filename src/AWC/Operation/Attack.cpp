#include <AWC/Operation/Attack.h>
#include <AWC/Operation/TakeDmg.h>
#include <AWC/Operation/StatMod.h>

#include <AWC/Game.h>

using namespace Operation;

Result Attack::Execute(Game& game)
{
    Result result{SUCCESS};

    // TODO: Check if the unit can attack there

    auto& map = game.GetMap(origin_.mapIndex);
    unit_ = map.GetUnit(origin_.pos);

    // TODO: Get units inside attack (it can be an AoE)
    // TODO: Check if the weapon has enough ammo

    if(auto targetUnit = map.GetUnit(dest_.pos))
    {
        auto dmg = unit_->GetDmgToUnit(weaponIndex_, targetUnit);
        OperationIPtr op{new TakeDmg{targetUnit, dmg, this}};
        game.Push(op);
    }

    // TODO: Weapon ammo is reduced
    StatMod::Extra extra;
    extra.ammo = {weaponIndex_};
    OperationIPtr ammoOp{new StatMod{unit_, UnitNS::AMMO, -1, extra}};
    game.Push(ammoOp);

    return result;
}