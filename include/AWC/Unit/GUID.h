#pragma once

#include <string>

namespace UnitNS
{
    struct GUID
    {
        GUID(unsigned int id, unsigned int typeId) : id{id}, typeId{typeId} {};

        bool operator==(const GUID& b) const
        {
            return id == b.id && typeId == b.typeId;
        }

        std::string ToString()
        {
            return std::to_string(id) + "-" + std::to_string(typeId);
        }

        unsigned int id;
        unsigned int typeId;
    };
}