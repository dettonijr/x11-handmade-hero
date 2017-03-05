#include "Point.h"
#include <vector>
#include <array>
#include "Framebuffer.hpp"
#include "TGAFile.hpp"

class Obj final{
public:
    Obj(const char * filename, const char* texture_file);
    ~Obj();
    void draw(Framebuffer& f, const Point<float>& light_vec, const Transform& t);    

private:
    TGAFile texture;
    std::vector<Point<float>> verts;
    std::vector<Point<float>> texture_points;
    std::vector<std::array<uint32_t, 3>> faces;
    std::vector<std::array<uint32_t, 3>> faces_texture;
};
