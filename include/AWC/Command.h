#pragma once

#include <exception>
#include <string>

class Map;

class Command
{
public:
    virtual ~Command(){};
    void Execute();
    virtual bool CanBeExecuted() = 0;
private:
    virtual void DoExecute() = 0;
};

class MoveCommand : public Command
{
public:
    MoveCommand(Map& map, int originX, int originY, int destX, int destY);

    bool CanBeExecuted() override;
private:

    virtual void DoExecute() override;

    Map& map_;
    const int originX_;
    const int originY_;
    const int destX_;
    const int destY_;
};

class InvalidCommandException : public std::exception
{
public:
    InvalidCommandException(const std::string error);
    virtual char const* what() const noexcept override;
private:
    const std::string error_;
};