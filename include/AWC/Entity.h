#include <AWC/Unit/GUID.h>

#include <variant>

namespace Entity
{
    enum class Type
    {
        NIL,
        UNIT,
        TILE
    };

    union EntityU
    {
        EntityU() {};
        EntityU(UnitNS::GUID unitGUID) : unitGUID{unitGUID} {};

        UnitNS::GUID unitGUID;
    };

    struct Entity
    {
        Entity(Type type) : type{type} {};
        Entity(Type type, UnitNS::GUID guid) :
            type{type}, guid{guid} {};

        bool operator==(const Entity& b) const
        {
            return type == b.type && guid.unitGUID == b.guid.unitGUID;
        }

        Type type;
        EntityU guid;
    };

    static const Entity NIL{Type::NIL};
};