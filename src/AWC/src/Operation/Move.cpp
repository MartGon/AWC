#include <AWC/Operation/Move.h>

#include <AWC/Game.h>

using namespace Operation;

Result Move::Execute(Game& game, Process::Info info)
{
    Result result{SUCCESS};

    // TODO: Check if the unit can move there

    auto& map = game.GetMap(mapIndex_);
    unit_ = map.GetUnit(origin_);
    map.RemoveUnit(origin_);
    map.AddUnit(dest_, unit_);

    return result;
}