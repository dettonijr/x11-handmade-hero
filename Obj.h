#include "Point.h"
#include <vector>

class Obj {
public:
    Obj(const char * filename);
    ~Obj();

    std::vector<Point> verts;
    std::vector<std::vector<int> > faces;
};
