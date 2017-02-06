#ifndef POINT_H_
#define POINT_H_

template<typename T>
class Point {
public:
    T x, y, z;

    Point(T x0, T y0, T z0) : x(x0), y(y0), z(z0) {
    }

    ~Point() { 
    }
};

#endif
