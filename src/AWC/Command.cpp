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
            // TODO: Check player owns the units

            Vector2 origin{originX_, originY_};
            Vector2 dest{destX_, destY_};
            
            auto unitMovement = unit->CalculateMovement(map, origin);
            canBeExecuted = unitMovement.CanMove(dest);
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
            // TODO: Check player owns the units

            auto unitAttack = sourceUnit->CalculateAttack(weaponIndex_, map, unitIndex_);
            if(unitAttack.CanAttack(targetPos_))
            {
                if(auto targetUnit = map.GetUnit(targetPos_))
                {
                    // TODO: Check owner team. Team should be different. AW particular rule
                    // auto ownerIndex = targetUnit->getOwner()
                    // auto commandAuthor = game->GetPlayer(playerIndex)
                    // auto owner = game->GetPlayer(ownerIndex)
                    // canBeExecuted = commandAuthor.GetTeamId() != owner.GetTeamId()
                    canBeExecuted = true;
                }
                else
                    canBeExecuted = true;
            }
        }
    }

    return canBeExecuted;
}

// Exceptions

InvalidCommandException::InvalidCommandException(const std::string error) : AWCException(error)
{

}