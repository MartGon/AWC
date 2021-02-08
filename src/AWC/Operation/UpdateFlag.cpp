#include <AWC/Operation/UpdateFlag.h>

#include <AWC/Game.h>

using namespace Operation;

Result UpdateFlag::Execute(Game& game)
{
    Result result{SUCCESS};

    set_ ? unit_->SetFlag(flag_) : unit_->RemoveFlag(flag_);

    switch (flag_)
    {
        case UnitNS::Flag::DEAD:
        {
            if(set_)
            {
                auto pos = game.GetUnitPos(unit_->GetGUID());
                if(pos)
                {
                    // Remove unit from map
                    auto& map = game.GetMap(pos->mapIndex);
                    map.RemoveUnit(pos->pos);

                    // Check if player lost
                    // TODO: Move to listener function of game or a new WinCondition class
                    auto owner = unit_->GetOwner();
                    if(game.HasPlayerLost(owner.GetId()))
                        game.OnPlayerLost(owner.GetId());
                }
            }

            break;
        }
        default:
            break;
    }

    return result;
}