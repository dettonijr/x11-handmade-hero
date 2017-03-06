#include "Point.h"
#include <vector>
#include <array>
#include "Framebuffer.hpp"
#include "TGAFile.hpp"

class Obj final{

    struct Vertex {
        Vertex(TGAFile& texture) : texture(texture) {
        }

        std::array<Point<float>, 3> v;
        std::array<Point<float>, 3> vt;
        TGAFile& texture;
    };

public:
    Obj(const char * filename, const char* texture_file);
    ~Obj();
    void draw(Framebuffer& f, const Point<float>& light_vec, const Transform& t);    

private:
    TGAFile texture;
    std::vector<Vertex> faces;
};
