// point.h
#ifndef POINT_H
#define POINT_H

template <typename T>
class Point {
public:
    Point(T x = 0, T y = 0) : x(x), y(y) {}

    T getX() const {return x;}
    void setX(T x) {this->x = x;}

    T getY() const {return y;}
    void setY(T y) {this->y = y;}
private:
    T x, y;
};

#endif 