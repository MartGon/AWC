#pragma once

#include <AWC/AWCException.h>
#include <AWC/AWCfwd.h>
#include <string>

class Command
{
public:
    virtual ~Command(){};
    void Execute(Game& game, uint playerIndex);
    virtual bool CanBeExecuted(Game& game, uint playerIndex) = 0;
private:
    virtual void DoExecute(Game& game, uint playerIndex) = 0;
};

class MoveCommand : public Command
{
public:
    MoveCommand(uint mapIndex, int originX, int originY, int destX, int destY);

    bool CanBeExecuted(Game& game, uint playerIndex) override;
private:

    virtual void DoExecute(Game& game, uint playerIndex) override;

    const uint mapIndex_;
    const int originX_;
    const int originY_;
    const int destX_;
    const int destY_;
};

class AttackCommand : public Command
{
public:
    AttackCommand(uint mapIndex, Vector2 unitIndex, Vector2 targetPos, uint weaponIndex = 0);

    bool CanBeExecuted(Game& game, uint playerIndex) override;
private:

    virtual void DoExecute(Game& game, uint playerIndex) override;

    const uint mapIndex_;
    const Vector2 unitIndex_;
    const Vector2 targetPos_;
    const uint weaponIndex_;
};

class InvalidCommandException : public AWCException
{
public:
    InvalidCommandException(const std::string error);
};