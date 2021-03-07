#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Map.h>
#include <AWC/Player.h>
#include <AWC/Turn.h>
#include <AWC/Event.h>
#include <AWC/Operation/OperationI.h>
#include <AWC/Process.h>

#include <queue>
#include <optional>

struct Position
{
    Position(Vector2 pos, uint mapIndex) : pos{pos}, mapIndex{mapIndex} {}

    Vector2 pos;
    uint mapIndex;
};

class Game
{
public:

    // Players
    void AddPlayer(Player player);
    void RemovePlayer(uint playerIndex);
    Player& GetPlayer(uint playerIndex);
    std::vector<std::reference_wrapper<Player>> GetPlayersByTeam(uint teamId);
    uint GetPlayerCount() const;

    // Maps
    unsigned int AddMap(Map map);
    void RemoveMap(uint mapIndex);
    Map& GetMap(uint mapIndex);
    uint GetMapCount() const;

    // Commands
    bool CanExecuteCommand(CommandPtr command);
    bool CanExecuteCommand(CommandPtr command, uint playerIndex);
    void ExecuteCommand(CommandPtr command);
    void ExecuteCommand(CommandPtr command, uint playerIndex);

    // Units
    void AddUnit(UnitPtr unit, Vector2 pos, uint mapIndex = 0);
    void RemoveUnit(Vector2 pos, uint mapIndex = 0);

    // Operation
    void RemoveProcess(unsigned int id);
    std::optional<Process> GetProcess(unsigned int id);
    unsigned int Push(OperationIPtr op, uint8_t prio = PRIORITY_DEFAULT);
    void Run();

    // State
    void Start();
    bool IsOver() const;

    uint GetWinnerTeamId();

    void OnPlayerLost(uint playerIndex);
    bool HasPlayerLost(uint playerIndex) const;
    bool HasPlayerBeenRouted(uint playerIndex) const;

    // Queries
    void EnumUnits(std::function<void(UnitPtr)> operation, uint mapIndex = 0) const;
    void EnumUnits(std::function<void(UnitPtr, Position)> operation, uint mapIndex = 0) const;

    std::vector<UnitPtr> GetUnits(uint mapIndex = 0) const;
    std::vector<UnitPtr> GetPlayerUnits(uint playerIndex, uint mapIndex = 0) const;
    UnitPtr GetUnit(Entity::GUID guid);
    std::optional<Position> GetUnitPos(Entity::GUID guid);

    // Turn history
    const Turn& GetCurrentTurn() const;
    void PassTurn();

    // Events
    Event::Subject& GetSubject();

private:
    
    // Events
    void NotifyPassTurn(Turn& turn);

    // Index checks
    bool IsPlayerIndexValid(uint playerIndex) const;
    bool IsMapIndexValid(uint mapIndex) const;
    void CheckPlayerIndex(uint playerIndex) const;
    void CheckMapIndex(uint mapIndex) const;

    // Operations
    void SortQueue();

    std::vector<Player> players_;
    std::vector<Map> maps_;

    std::vector<Process> processQueue_;
    std::vector<Process> opHistory_;
    unsigned int nextProcessId = 0;
    
    Turn currentTurn;    
    Event::Subject events;
};