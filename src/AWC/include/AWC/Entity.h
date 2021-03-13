#pragma once

#include <variant>
#include <string>

namespace Entity
{
    enum class Type
    {
        NIL,
        UNIT,
        TILE,
        HANDLER
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
            unsigned int type = static_cast<unsigned int>(this->type);
            return std::to_string(type) + "-" + std::to_string(id) + "-" + std::to_string(subType);
        }

        Entity::Type type;
        unsigned int subType;
        unsigned int id;
    };

    static const GUID NIL{Type::NIL, 0, 0};
};