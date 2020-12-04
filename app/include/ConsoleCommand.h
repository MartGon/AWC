#pragma once

#include <Console.h>

#include <AWC/Game.h>

class ConsoleCommand
{
public:
    virtual ~ConsoleCommand() {};

    virtual std::string GetHelpMessage() { return std::string{};};

    virtual void Execute(std::vector<std::string> args) = 0;
};

class ExitConsoleCommand : public ConsoleCommand
{
public:
    ExitConsoleCommand(Console& console) : console_{console}
    {

    }

    void Execute(std::vector<std::string> args) override;
private:
    Console& console_;
};

class HelpConsoleCommand : public ConsoleCommand
{
public:

    HelpConsoleCommand(Console& console) : console_{console}
    {

    }

    void Execute(std::vector<std::string> args) override;

private:
    Console& console_;
};

class GameCommand : public ConsoleCommand
{
public:
    virtual ~GameCommand() {};
    GameCommand(Game& game) : game_{game}{};

    virtual void Execute(std::vector<std::string> args) override
    {

    };

protected:
    Game& game_;
};

struct Padding
{
    uint up;
    uint down;
    uint left;
    uint right;
};

class PrintMapCommand : public GameCommand
{
public:
    PrintMapCommand(Game& game, Padding padding) : padding_{padding}, GameCommand{game} {}; 

    void Execute(std::vector<std::string> args) override;
private:

    void PrintPadding(uint length, char c);
    Padding padding_;
};

class PassTurnCommand : public GameCommand
{
public:
    PassTurnCommand(Game& game) : GameCommand{game} {}

    void Execute(std::vector<std::string> args) override;
};

class UnitMoveCommand : public GameCommand
{
public:
    UnitMoveCommand(Game& game) : GameCommand(game){}

    void Execute(std::vector<std::string> args) override;
private:
    const unsigned int ARGS_SIZE = 4;
};

class UnitAttackCommand : public GameCommand
{
public:
    UnitAttackCommand(Game& game) : GameCommand{game} {}

    void Execute(std::vector<std::string> args) override;

private:
    const unsigned int ARGS_SIZE = 4;
};

class UnitReportCommand : public GameCommand
{
public:
    UnitReportCommand(Game& game) : GameCommand(game){}

    void Execute(std::vector<std::string> args);

private:
    const unsigned int ARGS_SIZE = 2;
};

