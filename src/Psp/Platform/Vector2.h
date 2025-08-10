#ifndef VECTOR2_H
#define VECTOR2_H

template<typename T>
struct Vector2 {
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}

    Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
    Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    Vector2<T> operator*(const T factor) const { return Vector2<T>(x * factor, y * factor); }
    Vector2<T> operator/(const T factor) const { return Vector2<T>(x / factor, y / factor); }

    inline void SetXY(T x, T y) { this->x = x; this->y = y; }
};

#endif //VECTOR2_H
