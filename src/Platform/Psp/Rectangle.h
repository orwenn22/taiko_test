#ifndef RECTANGLE_H
#define RECTANGLE_H

template <typename T>
struct Rectangle {
    T x, y, w, h;

    Rectangle() : x(0), y(0), w(0), h(0) {}
    Rectangle(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    Rectangle(const Rectangle &other) : x(other.x), y(other.y), w(other.w), h(other.h) {}
};


#endif //RECTANGLE_H
