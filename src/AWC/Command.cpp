#include <AWC/Command.h>

#include <AWC/Game.h>
#include <AWC/UnitMovement.h>
#include <AWC/UnitAttack.h>

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
    mapIndex_{mapIndex}, origin_{originX, originY}, dest_{destX, destY}
{

}

MoveCommand::MoveCommand(uint mapIndex, Vector2 origin, Vector2 dest) : 
    mapIndex_{mapIndex}, origin_{origin}, dest_{dest}
{

}

void MoveCommand::DoExecute(Game& game, uint playerIndex)
{
    auto& map = game.GetMap(mapIndex_);
    auto unit = map.GetUnit(origin_);
    map.RemoveUnit(origin_);
    map.AddUnit(dest_, unit);

    auto unitMovement = unit->CalculateMovement(map, origin_);
    unit->Move(unitMovement.GetMoveCostTo(dest_));
}

bool MoveCommand::CanBeExecuted(Game& game, uint playerIndex)
{
    bool canBeExecuted = false;

    auto map = game.GetMap(mapIndex_);
    bool mapOK = map.IsPositionValid(origin_) && map.IsPositionValid(dest_) && map.IsPositionFree(dest_);
    if(mapOK)
    {
        if(auto unit = map.GetUnit(origin_))
        {
            auto commandAuthor = game.GetPlayer(playerIndex);
            if(unit->GetOwner().GetId() == commandAuthor.GetId())
            {
                auto unitMovement = unit->CalculateMovement(map, origin_);
                canBeExecuted = unitMovement.CanMove(dest_);
            }
        }
    }

    return canBeExecuted;    
}

// AttackCommand

AttackCommand::AttackCommand(uint mapIndex, Vector2 unitIndex, Vector2 targetPos, uint weaponIndex) 
    : mapIndex_{mapIndex}, unitIndex_{unitIndex}, targetPos_{targetPos}, weaponIndex_{weaponIndex}
{

}

void AttackCommand::DoExecute(Game& game, uint playerIndex)
{
    auto& map = game.GetMap(mapIndex_);
    
    auto sourceUnit = map.GetUnit(unitIndex_);

    // Unit takes damage
    if(auto targetUnit = map.GetUnit(targetPos_))
    {   
        auto dmg = sourceUnit->GetDmgToUnit(weaponIndex_, targetUnit);
        targetUnit->TakeDamage(dmg);
    }

    // TODO: Tile->OnAttack(): Tile triggers effect when attacked

    // Weapon ammo is reduced
    sourceUnit->UseWeapon(weaponIndex_);
}

bool AttackCommand::CanBeExecuted(Game& game, uint playerIndex)
{
    bool canBeExecuted = false;

    auto& map = game.GetMap(mapIndex_);

    bool mapOK = map.IsPositionValid(unitIndex_) && map.IsPositionValid(targetPos_);
    if(mapOK)
    {
        if(auto sourceUnit = map.GetUnit(unitIndex_))
        {
            auto commandAuthor = game.GetPlayer(playerIndex);
            if(sourceUnit->GetOwner().GetId() == commandAuthor.GetId())
            {
                auto unitAttack = sourceUnit->CalculateAttack(weaponIndex_, map, unitIndex_);
                if(unitAttack.CanAttack(targetPos_))
                {
                    if(auto targetUnit = map.GetUnit(targetPos_))
                    {
                        // Team should be different
                        auto owner = targetUnit->GetOwner();
                        canBeExecuted = commandAuthor.GetTeamId() != owner.GetTeamId();
                    }
                    else
                        canBeExecuted = true;
                }
            }
        }
    }

    return canBeExecuted;
}

// Exceptions

InvalidCommandException::InvalidCommandException(const std::string error) : AWCException(error)
{

}