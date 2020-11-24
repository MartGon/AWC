#include <Console.h>
#include <ConsoleCommand.h>

#include <Utils/STLUtils.h>

Console::Console(Game& game) : game_{game}, open_{true}
{

}

// Public

void Console::SetPromptMsg(std::string promptMsg)
{
    this->promptMsg_ = promptMsg;
}

void Console::AddCommand(std::string ref, std::shared_ptr<ConsoleCommand> command)
{
    commands_[ref] = command;
}

void Console::Prompt()
{
    std::cout << promptMsg_ << '\n';
    std::string line;
    std::getline(std::cin >> std::ws, line);

    std::cout << '\n';
    ParseCommand(line);
}

bool Console::IsOpen()
{
    return open_;
}

// Private

CommandInfo Console::ParseCommand(std::string line)
{
    CommandInfo ci;
    // TODO
}

void Console::ExecuteCommand(CommandInfo ci)
{
    if(UnorderedMapUtils::Contains(commands_, ci.commandRef))
        commands_[ci.commandRef]->Execute(game_, {});
    else
        std::cout << "Sorry, command " << ci.commandRef << " does not exist";
}