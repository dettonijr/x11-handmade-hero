#ifndef POINT_H_
#define POINT_H_

#include <cmath>

template<typename T>
class Point {
public:
    T x, y, z;

    Point(T x0, T y0, T z0) : x(x0), y(y0), z(z0) {
    }

    ~Point() { 
    }

    Point<T> operator-(const Point<T>& o) {
        return Point<T>(x-o.x, y-o.y, z-o.z);
    }
    
    Point<T> operator+(const Point<T>& o) {
        return Point<T>(x+o.x, y+o.y, z+o.z);
    }
    
    Point<T> operator*(float c) {
        return Point<T>(x*c, y*c, z*c);
    }
    
    T operator*(const Point<T>& o) {
        return x*o.x + y*o.y + z*o.z;
    }

    float norm() { 
        return std::sqrt(x*x + y*y + z*z); 
    }

    void normalize(T len = 1) {
        *this = (*this)*(1/norm());
    }
};

#endif
