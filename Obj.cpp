#include "Obj.h"
#include <string>
#include <sstream>
#include <fstream>


Obj::Obj(const char * filename, const char* texture_file) : texture(texture_file) {
    std::ifstream in(filename, std::ifstream::in);

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream ss(line.c_str());
        std::string command;
        ss >> command;
        if (command == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            Point<float> p(x, y, z);
            verts.push_back(p);
        } else if (command == "vt") { 
            float x, y, z;
            ss >> x >> y >> z;
            Point<float> p(x, y, z);
            texture_points.push_back(p);
        } else if (command == "f") { 
            std::array<uint32_t, 3> f;
            std::array<uint32_t, 3> ftexture;
            int idx;
            int idxtexture;
            char trash;
            int itrash;
            uint32_t currFaceComponent = 0;
            while(ss >> idx) {
                ss >> trash;
                ss >> idxtexture;
                ss >> trash;
                ss >> itrash;
                idx--;
                idxtexture--;
                f[currFaceComponent] = idx;
                ftexture[currFaceComponent] = idxtexture;
                ++currFaceComponent;
            }
            faces.push_back(f);
            faces_texture.push_back(ftexture);
        }
    }
}

Obj::~Obj() 
{

}
   
void Obj::draw(Framebuffer& f, const Point<float>& lightVec, const Transform& t) {
    int width = f.width();
    int height = f.height();

    for (int i = 0; i < faces.size(); i++) {
        auto face = faces[i];
        auto face_texture = faces_texture[i];
        Point<float>& v0 = verts[face[0]];
        Point<float>& v1 = verts[face[1]];
        Point<float>& v2 = verts[face[2]];
        Point<float>& t0 = texture_points[face_texture[0]];
        Point<float>& t1 = texture_points[face_texture[1]];
        Point<float>& t2 = texture_points[face_texture[2]];
        
        Point<float> p0 = v0*t;
        Point<float> p1 = v1*t;
        Point<float> p2 = v2*t;

        Point<float> a = p2-p0; 
        Point<float> b = p1-p0;

        float nx = a.y*b.z - a.z*b.y; 
        float ny = a.z*b.x - a.x*b.z; 
        float nz = a.x*b.y - a.y*b.x; 

        const Point<float> n(nx,ny,nz);
        const Point<float> normalized = n.normalize();
        float intensity = normalized*lightVec;

        if (intensity > 0) {
            TextureShader s{p0, p1, p2, t0, t1, t2, intensity, texture};
            //FlatShader s{Color::White*intensity};
            f.draw_triangle(p0, p1, p2, s); 
        }

    }

}
