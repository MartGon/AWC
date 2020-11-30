#include <ConsoleCommand.h>

#include <AWC/Command.h>

// Exit

void ExitConsoleCommand::Execute(std::vector<std::string> args)
{
    console_.Close();
}

// Help

void HelpConsoleCommand::Execute(std::vector<std::string> args)
{
    console_.Help();
}

// Print Map

void PrintMapCommand::Execute(std::vector<std::string> args)
{
    auto& map = game_.GetMap(0);
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
                std::string color = unit->GetOwner().GetId() == 1 ? BLUE_FG : RED_FG;
                std::string str = color + std::string(1, c);
                auto substr = charMatrix[i][j].substr(0, ESCAPE_SIZE);
                charMatrix[i][j] = substr + str;
            }
        }
    }

    // Upper padding
    PrintPadding(padding_.up, '\n');

    // Print X coords
    PrintPadding(padding_.left + 1, ' ');
    for(int j = 0; j < mapSize.x; j++)
    {
        std::cout << j;
    }
    std::cout << '\n';

    // Print matrix
    for(int i = 0; i < mapSize.y; i++)
    {   
        PrintPadding(padding_.left, ' ');
        std::cout << i;

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

// Pass turn

void PassTurnCommand::Execute(std::vector<std::string> args)
{
    auto turn = game_.GetCurrentTurn();
    std::cout << "Player " << turn.playerIndex << " turn has passed\n";

    game_.PassTurn();

    auto nextTurn = game_.GetCurrentTurn();
    std::cout << "Now it's Player " << nextTurn.playerIndex << " turn\n";
}

// Move Unit

void UnitMoveCommand::Execute(std::vector<std::string> args)
{
    if(args.size() >= ARGS_SIZE)
    {
        int originX = std::atoi(args[0].c_str());
        int originY = std::atoi(args[1].c_str());
        Vector2 origin{originX, originY};

        int destX = std::atoi(args[2].c_str());
        int destY = std::atoi(args[3].c_str());
        Vector2 dest{destX, destY};

        CommandPtr moveCommand{new MoveCommand{0, {origin}, {dest}}};
        if(game_.CanExecuteCommand(moveCommand))
        {
            game_.ExecuteCommand(moveCommand);

            auto& map = game_.GetMap(0);
            auto unit = map.GetUnit(destX, destY);

            std::cout << unit->GetName() << " moved successfully from " + origin + " to " + dest + "\n";
        }
        else
            std::cout << "Sorry move command could not be executed\n";
    }
    else
        std::cout << "Incorrect number of arguments for MoveCommand\n";
}

// Unit attack

void UnitAttackCommand::Execute(std::vector<std::string> args)
{
    if(args.size() >= ARGS_SIZE)
    {
        int originX = std::atoi(args[0].c_str());
        int originY = std::atoi(args[1].c_str());
        Vector2 origin{originX, originY};

        int destX = std::atoi(args[2].c_str());
        int destY = std::atoi(args[3].c_str());
        Vector2 dest{destX, destY};

        CommandPtr attackComm{new AttackCommand{0, origin, dest}};
        if(game_.CanExecuteCommand(attackComm))
        {
            // Info
            auto& map = game_.GetMap(0);
            auto attacker = map.GetUnit(origin);
            auto victim = map.GetUnit(dest);

            auto rawDmg = attacker->GetDmgToUnit(0, victim);
            auto realDmg = victim->GetDmgTaken(rawDmg);

            game_.ExecuteCommand(attackComm);

            std::cout << attacker->GetName() << " attacks " << victim->GetName() << " dealing " << realDmg << " damage to it\n";
            if(victim->IsDead())
                std::cout << victim->GetName() << " was destroyed by " << attacker->GetName() << "'s attack\n";
        }
        else
            std::cout << "Sorry. AttackCommand could not be executed\n";
    }
    else
        std::cout << "Sorry, not enough arguments for that command\n";
}

// Unit report command

void UnitReportCommand::Execute(std::vector<std::string> args)
{
    if(args.size() >= ARGS_SIZE)
    {
        int originX = std::atoi(args[0].c_str());
        int originY = std::atoi(args[1].c_str());
        Vector2 origin{originX, originY};

        auto& map = game_.GetMap(0);
        if(map.IsPositionValid(origin))
        {
            if(auto unit = map.GetUnit(origin))
            {
                auto name = unit->GetName();
                auto health = unit->GetHealth();
                auto ammo = unit->GetWeaponAmmo(0);
                auto gas = unit->GetCurrentGas();
                auto canAttack = unit->CanAttack();
                auto canMove = unit->CanMove();

                std::cout << '\n';
                std::cout << "Unit name: " << name << '\n';
                std::cout << "Unit health: " << health << '\n';
                std::cout << "Unit can attack: " << canAttack << '\n';
                std::cout << "Unit can move: " << canMove << '\n';
                std::cout << "Unit ammo: " << ammo << '\n';
                std::cout << "Unit gas: " << gas << '\n';
                std::cout << '\n';
            }
            else
                std::cout << "Could not find Unit at " + origin << '\n';
        }
        else
            std::cout << "Error: Invalid position: " + origin + "\n";
    }
    else
        std::cout << "Incorrect number of arguments for UnitReportCommand\n";
    
}