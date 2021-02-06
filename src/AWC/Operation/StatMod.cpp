#include <AWC/Operation/StatMod.h>
#include <AWC/Unit/Unit.h>

using namespace Operation;
using namespace UnitNS;

Result StatMod::Execute(Game& game)
{
    Result res{SUCCESS};

    switch(type_)
    {
        case GAS:
            unit_->Move(amount_);
            break;
        default:
            break;
    }

    return res;
}