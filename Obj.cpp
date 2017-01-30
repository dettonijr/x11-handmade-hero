#include "Obj.h"
#include <string>
#include <sstream>
#include <fstream>


Obj::Obj(const char * filename) {
    std::ifstream in(filename, std::ifstream::in);

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream ss(line.c_str());
        char command;
        ss >> command;
        if (command == 'v') {
            float x, y, z;
            ss >> x >> y >> z;
            Point p(x, y, z);
            verts.push_back(p);
        } else if (command == 'f') { 
            std::vector<int> f;
            int idx;
            char trash;
            int itrash;
            while (ss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--;
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
}

Obj::~Obj() 
{

}
   
void Obj::draw(Framebuffer& f) {
    int width = f.width();
    int height = f.height();
    for (int i = 0; i < faces.size(); i++) {
        std::vector<int> face = faces[i];
        for (int j = 0; j < 3; j++) {
            Point v0 = verts[face[j]];
            Point v1 = verts[face[(j+1)%3]];
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            Color c(0x00, 0x00, 0x00);
            f.draw_line(x0, y0, x1, y1, c);
        }
    }

}
