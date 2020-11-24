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

#define RESET "\033[0m"

// Foregrounds
#define RED_FG "\x1B[31m"
#define BLUE_FG "\x1B[34m"

// Backgrounds
#define GREEN_BG "\033[42m"

#define ESCAPE_SIZE 5

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

void PrintMap(const Map& map)
{
    auto mapSize = Vector2{map.GetWidth(), map.GetHeight()};

    // Init char matrix
    std::vector<std::vector<std::string>> charMatrix;
    charMatrix.resize(mapSize.x);
    for(int i = 0; i < mapSize.x; i++)
        charMatrix[i] = std::vector<std::string>(mapSize.y);

    // Edit matrix with tiles
    for(int i = 0; i < mapSize.x; i++)
    {
        for(int j = 0; j < mapSize.y; j++)
        {
            auto tile = map.GetTile(i, j);
            std::string str = GREEN_BG + std::string(1, ' ');
            charMatrix[i][j] = str;
        }
    }

    // Edit matrix with units
    for(int i = 0; i < mapSize.x; i++)
    {
        for(int j = 0; j < mapSize.y; j++)
        {
            auto unit = map.GetUnit(i, j);
            if(unit)
            {
                char c = unit->GetName()[0];
                std::string color = i > 2 ? BLUE_FG : RED_FG;
                std::string str = color + std::string(1, c);
                auto substr = charMatrix[i][j].substr(0, ESCAPE_SIZE);
                charMatrix[i][j] = substr + str;
            }
        }
    }

    // Print matrix
    for(int i = 0; i < mapSize.y; i++)
    {   
        for(int j = 0; j < mapSize.x; j++)
        {
            auto str = charMatrix[j][i];
            std::cout << str + RESET;
        }
        std::cout << '\n';
    }
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
    PrintMap(game.GetMap(0));

    return 0;
}