#include <AWC/Command.h>
#include <AWC/Map.h>
#include <AWC/UnitMovement.h>

void Command::Execute()
{
    // TODO: if already checked and valid, proceed directly. Creater member variable for that or something
    // It should be much more eficient than checking again.
    if(CanBeExecuted())
    {
        DoExecute();
    }
    else
        // TODO: GetLastError or something similar from the object itself. One alternative: 
        // CanBeExecuted could return info 
        throw InvalidCommandException("Command::Execute:Error");
}

// MoveCommand

MoveCommand::MoveCommand(Map& map, int originX, int originY, int destX, int destY) : 
    map_{map}, originX_{originX}, originY_{originY}, destX_{destX}, destY_{destY}
{

}

void MoveCommand::DoExecute()
{
    auto unit = map_.GetUnit(originX_, originY_);
    map_.RemoveUnit(originX_, originY_);
    map_.AddUnit(destX_, destY_, unit);
}

bool MoveCommand::CanBeExecuted()
{
    bool canBeExecuted = false;

    bool mapOK = map_.IsPositionValid(originX_, originY_) && map_.IsPositionValid(destX_, destY_) && map_.IsPositionFree(destX_, destY_);
    if(mapOK)
    {
        if(auto unit = map_.GetUnit(originX_, originY_))
        {
            Vector2 origin{originX_, originY_};
            Vector2 dest{destX_, destY_};
            
            auto unitMovement = unit->CalculateMovement(map_, origin);
            canBeExecuted = unitMovement.CanMove(dest);
        }
    }

    return canBeExecuted;    
}

// Exceptions

InvalidCommandException::InvalidCommandException(const std::string error) : AWCException(error)
{

}