#include "AWC/Operation/OperationI.h"

#include "AWC/AWCusing.h"

namespace Operation
{
    class StatMod : public OperationI
    {
    public:
        ~StatMod() override {};
        StatMod(UnitPtr unit, UnitNS::StatType type, int amount) : unit_{unit}, type_{type}, amount_{amount}, OperationI{Type::STAT_MOD} {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        UnitNS::StatType type_;
        int amount_;
    };
}