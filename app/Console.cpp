#include <Console.h>
#include <ConsoleCommand.h>

#include <Utils/STLUtils.h>

Console::Console(Game& game) : open_{true}
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
    auto ci = ParseCommand(line);
    ExecuteCommand(ci);
}

bool Console::IsOpen()
{
    return open_;
}

void Console::Close()
{
    open_ = false;
}

std::vector<std::string> Console::GetAvailableCommands() const
{
    return UnorderedMapUtils::GetKeys(commands_);
}

void Console::Help()
{
    auto commands = this->GetAvailableCommands();
    std::cout << "Available commands are:\n";
    std::cout << '\n';

    for(const auto& command : commands)
        std::cout << command << '\n';
    
    std::cout << '\n';
}

// Private

CommandInfo Console::ParseCommand(std::string line)
{
    
    char delim = ' ';
    auto items = StringUtils::Split(line, delim);

    CommandInfo ci;
    ci.commandRef = items[0];
    ci.args = VectorUtils::Slice(items, 1, items.size() - 1);

    return ci;
}

void Console::ExecuteCommand(CommandInfo ci)
{
    if(UnorderedMapUtils::Contains(commands_, ci.commandRef))
        commands_[ci.commandRef]->Execute(ci.args);
    else
    {
        std::cout << "Sorry, command " << ci.commandRef << " does not exist\n";
        Help();
    }
}