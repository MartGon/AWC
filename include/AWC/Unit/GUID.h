#pragma once

namespace UnitNS
{
    struct GUID
    {
        GUID(unsigned int id, unsigned int typeId) : id{id}, typeId{typeId} {};

        bool operator==(const GUID& b) const
        {
            return id == b.id && typeId == b.typeId;
        }

        unsigned int id;
        unsigned int typeId;
    };
}