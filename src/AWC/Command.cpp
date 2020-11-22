#include <AWC/Command.h>

#include <AWC/Game.h>
#include <AWC/UnitMovement.h>

void Command::Execute(Game& game, uint playerIndex)
{
    // TODO: if already checked and valid, proceed directly. Creater member variable for that or something
    // It should be much more eficient than checking again.
    if(CanBeExecuted(game, playerIndex))
    {
        DoExecute(game, playerIndex);
    }
    else
        // TODO: GetLastError or something similar from the object itself. One alternative: 
        // CanBeExecuted could return info 
        throw InvalidCommandException("Command::Execute:Error");
}

// MoveCommand

MoveCommand::MoveCommand(uint mapIndex, int originX, int originY, int destX, int destY) : 
    mapIndex_{mapIndex}, originX_{originX}, originY_{originY}, destX_{destX}, destY_{destY}
{

}

void MoveCommand::DoExecute(Game& game, uint playerIndex)
{
    auto& map = game.GetMap(mapIndex_);
    auto unit = map.GetUnit(originX_, originY_);
    map.RemoveUnit(originX_, originY_);
    map.AddUnit(destX_, destY_, unit);
}

bool MoveCommand::CanBeExecuted(Game& game, uint playerIndex)
{
    bool canBeExecuted = false;

    auto map = game.GetMap(mapIndex_);
    bool mapOK = map.IsPositionValid(originX_, originY_) && map.IsPositionValid(destX_, destY_) && map.IsPositionFree(destX_, destY_);
    if(mapOK)
    {
        if(auto unit = map.GetUnit(originX_, originY_))
        {
            Vector2 origin{originX_, originY_};
            Vector2 dest{destX_, destY_};
            
            auto unitMovement = unit->CalculateMovement(map, origin);
            canBeExecuted = unitMovement.CanMove(dest);
        }
    }

    return canBeExecuted;    
}

// Exceptions

InvalidCommandException::InvalidCommandException(const std::string error) : AWCException(error)
{

}