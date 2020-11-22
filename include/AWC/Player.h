
class Player
{
public:
    Player(unsigned int playerId, unsigned int teamId, unsigned int resources);

    unsigned int GetId() const;
    unsigned int GetTeamId() const;

    unsigned int GetResources() const;

private:
    unsigned int id_;
    unsigned int teamId_;

    unsigned int resources_;
};