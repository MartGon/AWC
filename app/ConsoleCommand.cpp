#include <ConsoleCommand.h>

PrintMapCommand::PrintMapCommand(Padding padding) : padding_{padding}
{

}

void PrintMapCommand::Execute(Game& game, std::vector<std::string> args)
{
    auto& map = game.GetMap(0);
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

    // Upper padding
    PrintPadding(padding_.up, '\n');

    // Print matrix
    for(int i = 0; i < mapSize.y; i++)
    {   
        PrintPadding(padding_.left, ' ');

        for(int j = 0; j < mapSize.x; j++)
        {
            auto str = charMatrix[j][i];
            std::cout << str + RESET;
        }
        PrintPadding(padding_.right, ' ');
        std::cout << '\n';
    }

    // Down padding
    PrintPadding(padding_.down, '\n');
}

void PrintMapCommand::PrintPadding(uint length, char c)
{
    for(int i = 0; i < length; i++)
        std::cout << c;
}