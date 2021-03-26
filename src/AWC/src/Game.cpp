#include <Utils.h>

#include <AWC/Game.h>
#include <AWC/Exception.h>
#include <AWC/Command.h>
#include <AWC/Operation/Operation.h>

#include <algorithm>

// Players

unsigned int Game::AddPlayer(Player player)
{
    players_.push_back(player);
    return GetPlayerCount() - 1;
}

unsigned int Game::AddPlayer(unsigned int teamId, unsigned int resources)
{
    auto id = GetPlayerCount();
    players_.emplace_back(id, teamId, resources);
    return id;
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

unsigned int Game::AddMap(Map map)
{
    maps_.push_back(map);
    return maps_.size() - 1;
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

// Units

void Game::AddUnit(UnitPtr unit, Vector2 pos, uint mapIndex)
{
    auto& map = GetMap(mapIndex);
    map.AddUnit(pos, unit);

    unit->RegisterHandlers(events);
}

void Game::RemoveUnit(Vector2 pos, uint mapIndex)
{
    auto& map = GetMap(mapIndex);
    auto unit = map.GetUnit(pos);
    if(unit)
    {
        auto guid = unit->GetGUID();
        events.Unregister(guid);

        map.RemoveUnit(pos);
    }
}

// Operation

void Game::RemoveProcess(unsigned int id)
{
    for(int index = 0; index < processQueue_.size(); index++)
    {   
        auto proc = processQueue_.at(index);
        if(proc.id == id)
        {
            VectorUtils::RemoveByIndex(processQueue_, index);
            break;
        }
    }

    return;
}

std::optional<Process> Game::GetProcess(unsigned int id)
{
    std::optional<Process> process;

    for(auto p : processQueue_)
    {
        if(p.id == id)
            return p;
    }

    return process;
}

unsigned int Game::Push(OperationIPtr op, uint8_t prio)
{
    unsigned int pid = nextProcessId++;
    processQueue_.push_back(Process{pid, op, prio});
    SortQueue();

    return pid;
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
        throw AWC::Exception("Either the game wasn't over or there are no players");
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

void Game::EnumUnits(std::function<void(UnitPtr, Position)> operation, uint mapIndex) const
{
    if(operation)
    {
        uint maps = GetMapCount();
        for(uint i = 0; i < maps; i++)
        {
            auto map = maps_.at(i);
            Vector2 mapSize = map.GetSize();
            for(int x = 0; x < mapSize.x; x++)
            {
                for(int y = 0; y < mapSize.y; y++)
                {
                    if(auto unit = map.GetUnit(x, y))
                        operation(unit, Position{{x, y}, i});
                }
            }
        }
    }
}

UnitPtr Game::GetUnit(Entity::GUID guid)
{
    UnitPtr unitPtr;

    auto myUnit = [&unitPtr, guid](UnitPtr unit, Position position)
    {
        if(unit->GetGUID() == guid)
            unitPtr = unit;
    };

    EnumUnits(myUnit);

    return unitPtr;
}

std::optional<Position> Game::GetUnitPos(Entity::GUID guid)
{
    std::optional<Position> pos;

    auto myUnit = [&pos, guid](UnitPtr unit, Position position)
    {
        if(unit->GetGUID() == guid)
            pos = position;
    };
    EnumUnits(myUnit);

    return pos;
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

// Events

Event::Subject& Game::GetSubject()
{
    return events;
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
    using namespace Event;

    while(!processQueue_.empty())
    {
        Process& ref = processQueue_.front();

        if(!ref.announced)
        {
            ref.announced = true;
            Notification::Notification notification{Notification::Type::PRE, ref};
            events.Notify(notification, *this);
        }

        // Take a copy. Taking a ref 
        Process process = processQueue_.front();
        if(process.announced)
        {
            Result res = process.op->Execute(*this, process.priority);
            Notification::Type type = Notification::Type::ERROR;

            if(res)
            {
                opHistory_.push_back(process);
                type = Notification::Type::POST;
            }
            processQueue_.erase(processQueue_.begin());

            Notification::Notification notification{type, process, res};
            events.Notify(notification, *this);
        }
    }
}

void Game::SortQueue()
{
    std::function<bool(Process a, Process b)> greater = [](Process a, Process b)
    {
        return a.priority > b.priority || (a.priority == b.priority && a.id < b.id);
    };
    std::sort(processQueue_.begin(), processQueue_.end(), greater);
}