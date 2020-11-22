#include <AWC/Game.h>
#include <Utils/STLUtils.h>
#include <AWC/AWCException.h>
#include <AWC/Command.h>

// Players

void Game::AddPlayer(Player player)
{
    players_.push_back(player);
}

void Game::RemovePlayer(uint playerIndex)
{
    VectorUtils::RemoveByIndex(players_, playerIndex);
}

Player& Game::GetPlayer(uint playerIndex)
{
    CheckPlayerIndex(playerIndex);
    return players_.at(playerIndex);
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

bool Game::CanExecuteCommand(CommandPtr command, uint playerIndex)
{
    return IsPlayerIndexValid(playerIndex) && command->CanBeExecuted(*this, playerIndex);
}

bool Game::CanExecuteCommand(CommandPtr command)
{
    return command->CanBeExecuted(*this, currentTurn.playerIndex);
}

void Game::ExecuteCommand(CommandPtr command, uint playerIndex)
{
    command->Execute(*this, playerIndex);
}

void Game::ExecuteCommand(CommandPtr command)
{
    command->Execute(*this, currentTurn.playerIndex);
}

// Private

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

// State

void Game::Start()
{
    currentTurn = Turn(0);
}

// Turn history

const Turn& Game::GetCurrentTurn() const
{
    return currentTurn;
}

void Game::PassTurn()
{
    auto lastPlayer = currentTurn.playerIndex;
    auto nextPlayer = (lastPlayer + 1) % players_.size();
    currentTurn = Turn(nextPlayer);
}