#pragma once

#include <vector>
#include <algorithm>
#include <array>
#include <string>

namespace VectorUtils
{
    template<typename T>
    bool IsInside(std::vector<T> vector, T value)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
    }

    template<typename T>
    void RemoveByValue(std::vector<T>& vector, T value)
    {
        vector.erase(std::remove(vector.begin(), vector.end(), value), vector.end());
    }

    template<typename T>
    std::vector<T> Union(const std::vector<T>& a, const std::vector<T> b)
    {
        std::vector<T> result;
        std::array<std::vector<T>, 2> vecs{a, b};
        for(const auto& vec : vecs)
            for(auto value : vec)
                if(!IsInside(result, value))
                    result.push_back(value);

        return result;
    }

    template <typename T>
    std::vector<T> Diff(const std::vector<T>& left, const std::vector<T> right)
    {
        std::vector<T> result;
        for(const auto& value : left)
            if(!IsInside(right, value))
                result.push_back(value);

        return result;
    }

    template <typename T>
    std::vector<T> Intersect(const std::vector<T>& a, const std::vector<T>& b)
    {
        std::vector<T> result;
        for(const auto& value : a)
            if(IsInside(b, value))
                result.push_back(value);
            
        return result;
    }
}

namespace StringUtils
{
    std::string Replace(std::string string, const std::string& token, const std::string& replacer);
}