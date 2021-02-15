#pragma once

#include <variant>
#include <string>

namespace Entity
{
    enum class Type
    {
        NIL,
        UNIT,
        TILE
    };

    struct GUID
    {
        GUID(Entity::Type type, unsigned int subType, unsigned int id) : type{type}, subType{subType}, id{id} {};

        bool operator==(const GUID& b) const
        {
            return type == b.type && subType == b.subType && id == b.id;
        }

        bool operator!=(const GUID& b) const
        {
            return !operator==(b);
        }

        std::string ToString()
        {
            unsigned int type = static_cast<unsigned int>(type);
            return std::to_string(type) + "-" + std::to_string(id) + "-" + std::to_string(subType);
        }

        Entity::Type type;
        unsigned int subType;
        unsigned int id;
    };

    static const GUID NIL{Type::NIL, 0, 0};

    union EntityU
    {
        EntityU() {};
        EntityU(GUID unitGUID) : unitGUID{unitGUID} {};

        Entity::GUID unitGUID;
    };

    struct Entity
    {
        Entity(Type type) : type{type} {};
        Entity(Type type, GUID guid) :
            type{type}, guid{guid} {};

        bool operator==(const Entity& b) const
        {
            return type == b.type && guid.unitGUID == b.guid.unitGUID;
        }

        Type type;
        EntityU guid;
    };
};