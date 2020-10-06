#include <vector>
#include <algorithm>

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
}