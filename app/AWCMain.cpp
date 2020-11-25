#include <iostream>
#include <AWC/Game.h>

// Tiles
#include <AWC/TileType.h>
#include <AWC/Tile.h>

// Units
#include <AWC/UnitType.h>
#include <AWC/TilePatternDesc.h>
#include <AWC/MovementDescType.h>
#include <AWC/CostTable.h>
#include <AWC/WeaponType.h>

// App
#include <Console.h>
#include <ConsoleCommand.h>

UnitType CreateSoldierType()
{
    // Name and id
    uint id = 0;
    std::string name = "Soldier";

    // TilePatternDesc
    std::vector<Vector2> manhattanMoves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    TilePatternDescIPtr manhattan = TilePatternDesc::Create(manhattanMoves);

    // CostTables
    std::shared_ptr<CostTable> unitCostTable{new CostTable};
    unitCostTable->SetCost(0, 0);

    CostTablePtr tileCostTable{new CostTable};
    uint grassId = 0;
    tileCostTable->SetCost(grassId, 1);

    // Movement
    MovementDecTypePtr moveType{ new MovementDescType{manhattan, {3, 0}, tileCostTable, unitCostTable, 99}};

    // Weapon
    AttackTable attackTable{ { {id, true}, {1, true} } };
    DamageTable damageTable{ { {id, 50}, {1, 25} } };
    WeaponTypePtr weaponType{ new WeaponType{manhattan, {1, 1}, attackTable, damageTable, 99}};

    UnitType soldierType{id, name, moveType, {weaponType}};

    return soldierType;
}



int main()
{
    // Prepare game
    Game game;

    // Tiles
    Map map{6, 3};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    // Units
    auto soldierType = CreateSoldierType();
    
        // Red units
    map.AddUnit({1, 0}, soldierType.CreateUnit());
    map.AddUnit({2, 1}, soldierType.CreateUnit());
    map.AddUnit({1, 2}, soldierType.CreateUnit());

        // Blue units
    map.AddUnit({4, 0}, soldierType.CreateUnit());
    map.AddUnit({4, 2}, soldierType.CreateUnit());

    // Set map
    game.AddMap(map);

    // Players
    game.AddPlayer(Player{0, 0, 1000});
    game.AddPlayer(Player{1, 1, 1000});

    // Finish prepare game
    Console console(game);

    // Prompt message
    std::string promptMsg = R"(
Welcome to AWC console.
Please, type a command.
)";
    console.SetPromptMsg(promptMsg);

    // Commands
    Padding padding{2, 2, 4, 2};
    std::shared_ptr<ConsoleCommand> printMapComm{new PrintMapCommand{game, padding}};
    std::shared_ptr<ConsoleCommand> exitComm{new ExitConsoleCommand{console}};
    std::shared_ptr<ConsoleCommand> moveComm{new MoveUnitGameCommand(game)};
    std::shared_ptr<ConsoleCommand> reportComm{new UnitReportCommand(game)};
    console.AddCommand("print", printMapComm);
    console.AddCommand("print-map", printMapComm);
    console.AddCommand("exit", exitComm);
    console.AddCommand("move", moveComm);
    console.AddCommand("report", reportComm);

    while(console.IsOpen())
        console.Prompt();

    return 0;
}