#pragma once

#include <Utils/STLUtils.h>
#include <optional>

#include <unordered_map>

namespace DB
{
    template<typename T>
    class Table
    {
    public:
        Table() : index_{0} {}
        Table(const Table& t) = delete;

        T* const GetById(unsigned int id)
        {
            T* entry = nullptr;
            if(UnorderedMapUtils::Contains(umap_, id))
                entry = &umap_.at(id);

            return entry;
        }

        int Add(T entry)
        {
            int id = index_++;
            umap_.insert({id, entry});
            return id;
        }

    private:
        std::unordered_map<unsigned int, T> umap_;
        unsigned int index_;
    };
}