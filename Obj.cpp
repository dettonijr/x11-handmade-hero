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
            printf("Reading v %d %d %d\n", x, y, z);
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
