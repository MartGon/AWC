#pragma once

#include <Utils/STLUtils.h>
#include <optional>
#include <functional>

#include <unordered_map>

template<typename T>
class Table
{
public:
    explicit Table() {};
    explicit Table(std::unordered_map<unsigned int, T> umap) : umap_{umap}
    {

    }
    
    T GetByIdCopy(unsigned int id) const
    {
        return umap_.at(id);
    }

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

    T* const GetFirstByCriteria(std::function<bool(const T&)> include)
    {
        T* ret = nullptr;
        for(const auto& entry : umap_)
            if(include(entry))
                ret = &entry;
        
        return ret;
    }

    std::vector<T* const> GetAllByCriteria(std::function<bool(const T&)> include)
    {
        std::vector<T* const>  entries;
        for(const auto& entry: umap_)
            if(include(entry))
                entries.push_back(&entry);
        return entries;
    }

    void RemoveById(unsigned int id)
    {
        umap_.erase(umap_.find(id));
    }

    void Insert(unsigned int id, T entry)
    {
        umap_.insert({id, entry});
    }

    int Add(T entry)
    {
        int id = GetFreeId();
        Insert(id, entry);
        return id;
    }

    void Set(unsigned int id, T entry)
    {
        umap_[id] = entry;
    }

    void Set(T entry)
    {
        umap_[entry.GetId()] = entry;
    }

    template <typename ...Args>
    int Emplace(Args&&... entry)
    {
        int id = GetFreeId();
        umap_.emplace(std::piecewise_construct, 
            std::forward_as_tuple(id), 
            std::forward_as_tuple(std::forward<Args>(entry)...));
        return id;
    }

    unsigned int GetFreeId() const
    {   
        unsigned int i = 0; 
        while(ContainsId(i)) 
            i++;

        return i;
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
};
