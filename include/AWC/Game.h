#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Map.h>
#include <AWC/Player.h>
#include <AWC/Turn.h>

class Game
{
public:

    // Players
    void AddPlayer(Player player);
    void RemovePlayer(uint playerIndex);
    Player& GetPlayer(uint playerIndex);
    uint GetPlayerCount() const;

    // Maps
    void AddMap(Map map);
    void RemoveMap(uint mapIndex);
    Map& GetMap(uint mapIndex);
    uint GetMapCount() const;

    // Commands
    bool CanExecuteCommand(CommandPtr command, uint playerIndex);
    bool CanExecuteCommand(CommandPtr command);
    void ExecuteCommand(CommandPtr command, uint playerIndex);
    void ExecuteCommand(CommandPtr command);

    // State
    void Start();
    bool IsOver() const;

    void OnPlayerLost(uint playerIndex);
    bool HasPlayerLost(uint playerIndex) const;
    bool HasPlayerBeenRouted(uint playerIndex) const;

    // Queries
    std::vector<UnitPtr> GetUnits(uint mapIndex = 0) const;
    std::vector<UnitPtr> GetPlayerUnits(uint playerIndex, uint mapIndex = 0) const;
    void EnumUnits(std::function<void(UnitPtr)> operation, uint mapIndex = 0) const;

    // Turn history
    const Turn& GetCurrentTurn() const;
    void PassTurn();

private:
    
    // Events
    void NotifyPassTurn(Turn& turn);

    // Index checks
    bool IsPlayerIndexValid(uint playerIndex) const;
    bool IsMapIndexValid(uint mapIndex) const;
    void CheckPlayerIndex(uint playerIndex) const;
    void CheckMapIndex(uint mapIndex) const;

    std::vector<Player> players_;
    std::vector<Map> maps_;
    Turn currentTurn;
};