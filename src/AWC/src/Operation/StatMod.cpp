#include <AWC/Operation/StatMod.h>
#include <AWC/Unit/Unit.h>

using namespace Operation;
using namespace UnitNS;

Result StatMod::Execute(Game& game, const Process::Info& info)
{
    Result res{SUCCESS};

    switch(type_)
    {
        case GAS:
            unit_->Move(amount_);
            break;
        case AMMO:
        {
            auto weaponIndex = extra_.ammo.weaponIndex;
            unit_->UseWeapon(weaponIndex);
        }
        default:
            break;
    }

    return res;
}