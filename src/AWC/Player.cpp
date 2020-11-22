#include <AWC/Player.h>

Player::Player(unsigned int playerId, unsigned int teamId, unsigned int resources) 
    : id_{playerId}, teamId_{teamId}, resources_{resources}
{

}

unsigned int Player::GetId() const
{
    return id_;
}

unsigned int Player::GetTeamId() const
{
    return teamId_;
}

unsigned int Player::GetResources() const
{
    return resources_;
}