#pragma once

#include <unordered_map>

#include <AWC/Exception.h>

#include <Utils.h>

template<typename T>
class Repository
{
public:
    Repository(){};
    Repository(Repository&) = delete;

    T& GetById(unsigned int id)
    {
        return entries.at(id);
    }

    void Add(unsigned int id, T entry)
    {
        if(!UnorderedMapUtils::Contains(entries, id))
            entries.insert({id, entry});
        else
            AWC::Exception("Repo: entry with id " + std::to_string(id) + " already exists");
    }

private:
    std::unordered_map<unsigned int, T> entries;
};