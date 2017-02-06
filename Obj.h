#include "Point.h"
#include <vector>
#include "Framebuffer.hpp"

class Obj {
public:
    Obj(const char * filename);
    ~Obj();
    void draw(Framebuffer& f);    

private:
    std::vector<Point<float>> verts;
    std::vector<std::vector<int> > faces;
};
