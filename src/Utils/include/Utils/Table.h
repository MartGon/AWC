#pragma once

#include <Utils/STLUtils.h>
#include <optional>

#include <unordered_map>

template<typename T>
class Table
{
public:
    Table() : index_{0} {}

    T* const GetById(unsigned int id)
    {
        T* entry = nullptr;
        if(UnorderedMapUtils::Contains(umap_, id))
            entry = &umap_.at(id);

        return entry;
    }

    std::vector<T> GetAll() const
    {
        std::vector<T> all;
        for(auto& entry : umap_)
            all.emplace_back(entry.second);
        
        return all;
    }

    void RemoveById(unsigned int id)
    {
        umap_.erase(umap_.find(id));
    }

    int Add(T entry)
    {
        int id = index_++;
        umap_.insert({id, entry});
        return id;
    }

    template <typename ...Args>
    int Emplace(Args&&... entry)
    {
        int id = index_++;
        umap_.emplace(std::piecewise_construct, 
            std::forward_as_tuple(id), 
            std::forward_as_tuple(std::forward<Args>(entry)...));
        return id;
    }

    unsigned int GetIndex() const
    {
        return index_;
    }

    bool ContainsId(unsigned int id) const
    {
        return umap_.find(id) != umap_.end();
    }

    unsigned int Size() const
    {
        return umap_.size();
    }

    auto begin() { return umap_.begin(); }
    auto end() { return umap_.end(); }

private:
    std::unordered_map<unsigned int, T> umap_;
    unsigned int index_;
};
