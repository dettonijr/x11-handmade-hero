#include "Point.h"
#include <vector>
#include "Framebuffer.hpp"

class Obj {
public:
    Obj(const char * filename);
    ~Obj();
    void draw(Framebuffer& f, const Point<float> light_vec, const Transform& t);    

private:
    std::vector<Point<float>> verts;
    std::vector<std::vector<int> > faces;
};
