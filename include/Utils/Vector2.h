#pragma once
#include <functional>
#include <string>

template<typename T>
class Vector2T
{
public:
    Vector2T(T x, T y) : x{x}, y{y} {};
    Vector2T() : x{0}, y{0} {};

    std::string ToString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    T x;
    T y;
};

using Vector2 = Vector2T<int>;

template<typename T>
inline Vector2T<T> operator+(const Vector2T<T>& a, const Vector2T<T>& b)
{
    return Vector2T<T>{a.x + b.x, a.y + b.y};
}

template <typename T>
inline std::string operator+(const std::string& str, const Vector2T<T>& vec)
{
    return str + vec.ToString();
}

template <typename T>
inline Vector2T<T> operator-(const Vector2T<T>& a)
{
    return Vector2T<T>{-a.x, -a.y};
}
template<typename T>
inline Vector2T<T> operator-(const Vector2T<T>& a, const Vector2T<T>& b)
{
    return Vector2T<T>{a.x - b.x, a.y - b.y};
}

template <typename T>
inline bool operator==(const Vector2T<T>& a, const Vector2T<T>& b)
{
    return a.x == b.x && a.y == b.y;
}

template <typename T>
inline bool operator!=(const Vector2T<T>& a, const Vector2T<T>& b)
{
    return a.x != b.x || a.y != b.y;
}

namespace std {

  template <>
  struct hash<Vector2>
  {
    std::size_t operator()(const Vector2& k) const
    {
        return ((std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1)) >> 1);
    };
  };

}