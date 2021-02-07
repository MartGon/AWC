#include <AWC/Game.h>
#include <Utils/STLUtils.h>
#include <AWC/AWCException.h>
#include <AWC/Command.h>
#include <AWC/Operation/Operation.h>

// Players

void Game::AddPlayer(Player player)
{
    players_.push_back(player);
}

void Game::RemovePlayer(uint playerIndex)
{
    VectorUtils::RemoveByIndex(players_, playerIndex);

    // TODO:  RemovePlayer Units;


    // TODO: Reset Captured buildings
}

Player& Game::GetPlayer(uint playerIndex)
{
    CheckPlayerIndex(playerIndex);
    return players_.at(playerIndex);
}

std::vector<std::reference_wrapper<Player>> Game::GetPlayersByTeam(uint teamId)
{
    std::vector<std::reference_wrapper<Player>> teamPlayers;
    for(auto& player : players_)
        if(player.GetTeamId() == teamId)
            teamPlayers.push_back(player);

    return teamPlayers;
}

uint Game::GetPlayerCount() const
{
    return players_.size();
}

// Maps

void Game::AddMap(Map map)
{
    maps_.push_back(map);
}

void Game::RemoveMap(uint mapIndex)
{
    VectorUtils::RemoveByIndex(maps_, mapIndex);
}

Map& Game::GetMap(uint mapIndex)
{
    CheckMapIndex(mapIndex);
    return maps_.at(mapIndex);
}

uint Game::GetMapCount() const
{
    return maps_.size();
}

// Commands

bool Game::CanExecuteCommand(CommandPtr command)
{
    return this->CanExecuteCommand(command, currentTurn.playerIndex);
}

bool Game::CanExecuteCommand(CommandPtr command, uint playerIndex)
{
    return IsPlayerIndexValid(playerIndex) && command->CanBeExecuted(*this, playerIndex);
}

void Game::ExecuteCommand(CommandPtr command)
{
    this->ExecuteCommand(command, currentTurn.playerIndex);
}

void Game::ExecuteCommand(CommandPtr command, uint playerIndex)
{
    command->Execute(*this, playerIndex);
    Run();
}

// Operation

void Game::Push(OperationIPtr op, uint8_t prio)
{
    opQueue_.push(Process{op, prio});
}

// State

void Game::Start()
{
    currentTurn = Turn(0);
}

bool Game::IsOver() const
{
    // Game is over when only players of the same team are left
    bool isNotOver = GetPlayerCount() > 1;
    if(isNotOver)
    {
        uint firstTeam = players_[0].GetTeamId();

        for(uint i = 1; i < GetPlayerCount() && isNotOver; i++)
        {
            isNotOver = players_.at(i).GetTeamId() != firstTeam;
        }
    }

    return !isNotOver;
}

uint Game::GetWinnerTeamId()
{
    if(IsOver() && GetPlayerCount() > 0)
        return players_[0].GetTeamId();
    else
        throw AWCException("Either the game wasn't over or there are no players");
}

void Game::OnPlayerLost(uint playerIndex)
{
    RemovePlayer(playerIndex);
}

bool Game::HasPlayerLost(uint playerIndex) const
{
    return HasPlayerBeenRouted(playerIndex);
}

bool Game::HasPlayerBeenRouted(uint playerIndex) const
{
    CheckPlayerIndex(playerIndex);

    uint units = 0;
    for(uint i = 0; i < GetMapCount(); i++)
        units += GetPlayerUnits(playerIndex, i).size();
    
    return units == 0;
}

// Queries

std::vector<UnitPtr> Game::GetUnits(uint mapIndex) const
{
    std::vector<UnitPtr> units;
    std::function<void(UnitPtr)> operation = [&units](UnitPtr unit)
    {
        units.push_back(unit);
    };
    EnumUnits(operation, mapIndex);

    return units;
}

std::vector<UnitPtr> Game::GetPlayerUnits(uint playerIndex, uint mapIndex) const
{
    std::vector<UnitPtr> playerUnits;
    std::function<void(UnitPtr)> operation = [&playerUnits, playerIndex](UnitPtr unit)
    {
        if(unit->GetOwner().GetId() == playerIndex)
            playerUnits.push_back(unit);
    };
    EnumUnits(operation, mapIndex);

    return playerUnits;
}

void Game::EnumUnits(std::function<void(UnitPtr)> operation, uint mapIndex) const
{
    if(operation)
    {
        auto map = maps_.at(mapIndex);
        Vector2 mapSize = map.GetSize();
        for(int x = 0; x < mapSize.x; x++)
        {
            for(int y = 0; y < mapSize.y; y++)
            {
                if(auto unit = map.GetUnit(x, y))
                    operation(unit);
            }
        }
    }
}

// Turn history

const Turn& Game::GetCurrentTurn() const
{
    return currentTurn;
}

void Game::PassTurn()
{
    // Increase turn
    auto lastPlayer = currentTurn.playerIndex;
    auto nextPlayer = (lastPlayer + 1) % players_.size();
    currentTurn = Turn(nextPlayer);

    // TODO: This is an example of possible event on an event system
    NotifyPassTurn(currentTurn);
}

// Private

    // Events

void Game::NotifyPassTurn(Turn &turn)
{
    auto units = GetUnits();
    for(auto& unit : units)
        unit->OnPassTurn(turn);
}

    // Index checks

bool Game::IsPlayerIndexValid(uint playerIndex) const
{
    return playerIndex < players_.size();
}

bool Game::IsMapIndexValid(uint mapIndex) const
{
    return mapIndex < maps_.size();
}

void Game::CheckPlayerIndex(uint playerIndex) const
{
    if(!IsPlayerIndexValid(playerIndex))
        throw std::out_of_range("Player index out of range:" + std::to_string(playerIndex));
}

void Game::CheckMapIndex(uint mapIndex) const
{
    if(!IsMapIndexValid(mapIndex))
        throw std::out_of_range("Map index out of range:" + std::to_string(mapIndex));
}

    // Operations

void Game::Run()
{
    using namespace Operation;

    while(!opQueue_.empty())
    {
        auto process = opQueue_.top();

        if(!process.announced)
        {
            events.Notify(process.op, Event::NotificationType::PRE);
            process.announced = true;
        }

        process = opQueue_.top();
        if(process.announced)
        {
            opQueue_.pop();

            Result res = process.op->Execute(*this);
            if(res)
                events.Notify(process.op, Event::NotificationType::POST);
        }
    }
}