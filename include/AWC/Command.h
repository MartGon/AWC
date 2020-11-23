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
    MoveCommand(uint mapIndex, Vector2 origin, Vector2 dest);

    bool CanBeExecuted(Game& game, uint playerIndex) override;
private:

    virtual void DoExecute(Game& game, uint playerIndex) override;

    const uint mapIndex_;
    const Vector2 origin_;
    const Vector2 dest_;
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