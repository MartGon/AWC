#pragma once

#include <AWC/AWCusing.h>

#include <AWC/Operation/StatMod.h>

namespace Operation
{
    class Factory
    {
    public:
        Factory() : nextId_{0} {};

        std::shared_ptr<Attack> CreateAttack(Position origin, Position dest, uint weaponIndex);
        std::shared_ptr<Composed> CreateComposed();
        std::shared_ptr<Custom> CreateCustom(std::function<void(Game&)> func);
        std::shared_ptr<Move> CreateMove(uint mapIndex, Vector2 origin, Vector2 dest);
        std::shared_ptr<StatMod> CreateStatMod(UnitPtr unit, UnitNS::StatType type, int amount, StatMod::Extra extra = StatMod::Extra{});
        std::shared_ptr<TakeDmg> CreateTakeDmg(UnitPtr victim, float attackDmg, OperationI* source);
        std::shared_ptr<UpdateFlag> CreateUpdateFlag(UnitPtr unit, UnitNS::Flag flag, bool set);

    private:

        unsigned int GetNextId();
        unsigned int nextId_;
    };
}