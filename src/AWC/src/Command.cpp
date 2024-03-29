#include <AWC/Command.h>

#include <AWC/Unit/Unit.h>
#include <AWC/Unit/MoveArea.h>
#include <AWC/Unit/AttackArea.h>

#include <AWC/Operation/Operation.h>

#include <Utils.h>

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

    auto unitMovement = unit->CalculateMovement(map, origin_);
    auto path = unitMovement.GetPathTo(dest_);

    Vector2 origin = origin_;
    VectorUtils::RemoveByValue(path, origin_);

    uint8_t prio = Process::PRIORITY_DEFAULT;
    for(auto tile : path)
    {
        Vector2 dest = tile;
        OperationIPtr move {new Operation::Move(mapIndex_, origin, dest)};
        OperationIPtr gasMod{new Operation::StatMod(unit, UnitNS::StatType::GAS, 1)};

        Process::Trigger::Trigger t{Process::Trigger::Type::PLAYER, playerIndex};
        game.Push(move, t, prio--);
        game.Push(gasMod, t, prio--);

        origin = dest;
    }
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
            if(unit->CanMove())
            {
                auto commandAuthor = game.GetPlayer(playerIndex);
                if(unit->GetOwner()->GetId() == commandAuthor->GetId())
                {
                    auto unitMovement = unit->CalculateMovement(map, origin_);
                    canBeExecuted = unitMovement.CanMove(dest_);
                }
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

    Position origin{unitIndex_, mapIndex_};
    Position dest{targetPos_, mapIndex_};
    OperationIPtr attack{ new Operation::Attack(origin, dest, weaponIndex_)};
    Process::Trigger::Trigger t{Process::Trigger::Type::PLAYER, playerIndex};
    game.Push(attack, t);
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
            if(sourceUnit->CanAttack())
            {
                auto commandAuthor = game.GetPlayer(playerIndex);
                if(sourceUnit->GetOwner()->GetId() == commandAuthor->GetId())
                {
                    auto unitAttack = sourceUnit->CalculateAttack(weaponIndex_, map, unitIndex_);
                    if(unitAttack.CanAttack(targetPos_))
                    {
                        if(auto targetUnit = map.GetUnit(targetPos_))
                        {
                            // Team should be different
                            auto owner = targetUnit->GetOwner();
                            canBeExecuted = commandAuthor->GetTeamId() != owner->GetTeamId();
                        }
                        else
                            canBeExecuted = true;
                    }
                }
            }
        }
    }

    return canBeExecuted;
}

// Exceptions

InvalidCommandException::InvalidCommandException(const std::string error) : AWC::Exception(error)
{

}