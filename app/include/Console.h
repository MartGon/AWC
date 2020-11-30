#pragma once

#include <iostream>

#include <AWC/Game.h>

class ConsoleCommand;

// Unicode console colors
// Source: https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal

#define RESET "\033[0m"

// Foregrounds
#define RED_FG "\x1B[31m"
#define BLUE_FG "\x1B[34m"

// Backgrounds
#define GREEN_BG "\033[42m"

#define ESCAPE_SIZE 5

struct CommandInfo
{
    std::string commandRef;
    std::vector<std::string> args;
};

class Console
{
public:
    Console(Game& game);

    // Config
    void SetPromptMsg(std::string msg);
    void AddCommand(std::string reference, std::shared_ptr<ConsoleCommand> command);

    void Prompt();
    bool IsOpen();
    void Close();

    void Help();

    std::vector<std::string> GetAvailableCommands() const;

private:

    CommandInfo ParseCommand(std::string line);
    void ExecuteCommand(CommandInfo ci);

    // Config
    std::string promptMsg_;
    std::unordered_map<std::string, std::shared_ptr<ConsoleCommand>> commands_;

    // State
    bool open_;
};