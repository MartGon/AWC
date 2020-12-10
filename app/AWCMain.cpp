#include <iostream>
#include <AWC/Game.h>

// Tiles
#include <AWC/TileType.h>
#include <AWC/Tile.h>

// Units
#include <AWC/Unit/UnitType.h>
#include <AWC/TilePattern/TilePatternDesc.h>
#include <AWC/Unit/MovementDescType.h>
#include <AWC/CostTable.h>
#include <AWC/Unit/WeaponType.h>

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
    unitCostTable->SetCost(0, std::numeric_limits<uint>::max());

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

        // Players
    Player playerOne{0, 0, 1000};
    Player playerTwo{1, 1, 1100};
    game.AddPlayer(playerOne);
    game.AddPlayer(playerTwo);

    // Tiles
    Map map{6, 3};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    // Units
    auto soldierType = CreateSoldierType();
    
        // Red units
    map.AddUnit({1, 0}, soldierType.CreateUnit(game.GetPlayer(0)));
    map.AddUnit({2, 1}, soldierType.CreateUnit(game.GetPlayer(0)));
    map.AddUnit({1, 2}, soldierType.CreateUnit(game.GetPlayer(0)));

        // Blue units
    map.AddUnit({4, 0}, soldierType.CreateUnit(game.GetPlayer(1)));
    //map.AddUnit({4, 2}, soldierType.CreateUnit(game.GetPlayer(1)));

    // Set map
    game.AddMap(map);

    // Finish prepare game
    Console console(game);

    // Prompt message
    std::string promptMsg = "Please, type a command.\n";
    console.SetPromptMsg(promptMsg);

    // Commands
    Padding padding{2, 2, 4, 2};
    std::shared_ptr<ConsoleCommand> printMapComm{new PrintMapCommand{game, padding}};
    std::shared_ptr<ConsoleCommand> exitComm{new ExitConsoleCommand{console}};
    std::shared_ptr<ConsoleCommand> moveComm{new UnitMoveCommand(game)};
    std::shared_ptr<ConsoleCommand> attackComm{new UnitAttackCommand(game)};
    std::shared_ptr<ConsoleCommand> reportComm{new UnitReportCommand(game)};
    std::shared_ptr<ConsoleCommand> passComm{new PassTurnCommand{game}};
    std::shared_ptr<ConsoleCommand> helpComm{new HelpConsoleCommand{console}};
    console.AddCommand("print", printMapComm);
    console.AddCommand("print-map", printMapComm);
    console.AddCommand("pass", passComm);
    console.AddCommand("exit", exitComm);
    console.AddCommand("move", moveComm);
    console.AddCommand("attack", attackComm);
    console.AddCommand("report", reportComm);
    console.AddCommand("help", helpComm);

    while(console.IsOpen())
    {
        console.Prompt();

        if(game.IsOver())
        {
            std::cout << "Game is over!!!\n";
            std::cout << "Team " << game.GetPlayer(0).GetTeamId() << " wins\n";
            break;
        }
    }

    return 0;
}