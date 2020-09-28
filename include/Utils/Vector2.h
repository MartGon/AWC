
template<typename T>
class Vector2T
{
public:
    Vector2T(T x, T y) : x{x}, y{y} {};

    T x;
    T y;
};

template<typename T>
inline Vector2T<T> operator+(const Vector2T<T>& a, const Vector2T<T>& b)
{
    return Vector2T<T>{a.x + b.x, a.y + b.y};
}

using Vector2 = Vector2T<int>;