#include <AWC/Unit/GUID.h>

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

        Type type;
        EntityU guid;
    };

    static const Entity NIL{Type::NIL};
};