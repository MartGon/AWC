#include <AWC/Operation/Factory.h>

#include <AWC/Operation/Operation.h>

using namespace Operation;

std::shared_ptr<Attack> Factory::CreateAttack(Position origin, Position dest, uint weaponIndex)
{
    return std::shared_ptr<Attack>{new Attack(GetNextId(), origin, dest, weaponIndex)};
}

std::shared_ptr<Composed> Factory::CreateComposed()
{
    return std::shared_ptr<Composed>{new Composed(GetNextId())};
}

std::shared_ptr<Custom> Factory::CreateCustom(std::function<void(Game&)> func)
{
    return std::shared_ptr<Custom>{new Custom(GetNextId(), func)};
}

std::shared_ptr<AntiOperation> Factory::CreateAntiOperation(unsigned int targetId)
{
    return std::shared_ptr<AntiOperation>{new AntiOperation(GetNextId(), targetId)};
}

std::shared_ptr<Move> Factory::CreateMove(uint mapIndex, Vector2 origin, Vector2 dest)
{
    return std::shared_ptr<Move>{new Move(GetNextId(), mapIndex, origin, dest)};
}

std::shared_ptr<StatMod> Factory::CreateStatMod(UnitPtr unit, UnitNS::StatType type, int amount, StatMod::Extra extra)
{
    return std::shared_ptr<StatMod>{new StatMod(GetNextId(), unit, type, amount, extra)};
}

std::shared_ptr<TakeDmg> Factory::CreateTakeDmg(UnitPtr victim, float attackDmg, OperationI* source)
{
    return std::shared_ptr<TakeDmg>{new TakeDmg(GetNextId(), victim, attackDmg, source)};
}

std::shared_ptr<UpdateFlag> Factory::CreateUpdateFlag(UnitPtr unit, UnitNS::Flag flag, bool set)
{
    return std::shared_ptr<UpdateFlag>{new UpdateFlag(GetNextId(), unit, flag, set)};
}

// Private

unsigned int Factory::GetNextId()
{
    return nextId_++;
}