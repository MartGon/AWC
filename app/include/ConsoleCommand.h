#pragma once

#include <Console.h>

#include <AWC/Game.h>


class ConsoleCommand
{
public:

    virtual void Execute(Game& game, std::vector<std::string> args) = 0;
};

struct Padding
{
    uint up;
    uint down;
    uint left;
    uint right;
};

class PrintMapCommand : public ConsoleCommand
{
public:
    PrintMapCommand(Padding padding);

    void Execute(Game& game, std::vector<std::string> args) override;
private:

    void PrintPadding(uint length, char c);

    Padding padding_;
};

class MoveUnitConsoleCommand : public ConsoleCommand
{
public:
    void Execute(Game& game, std::vector<std::string> args);
};