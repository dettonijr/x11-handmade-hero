#include "Point.h"
#include <vector>
#include "Framebuffer.hpp"
#include "TGAFile.hpp"

class Obj {
public:
    Obj(const char * filename, const char* texture_file);
    ~Obj();
    void draw(Framebuffer& f, const Point<float> light_vec, const Transform& t);    

private:
    TGAFile texture;
    std::vector<Point<float>> verts;
    std::vector<Point<float>> texture_points;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> faces_texture;
};
