#include "TGAFile.hpp"
#include <fstream>
#include <cassert>

TGAFile::TGAFile(const char* file) {
    assert(sizeof(header) == 18);

    std::ifstream in(file, std::ifstream::in);

    in.read((char*)&header, 18);
    data.reserve(header.width*header.height);

    while (data.size() <= header.width*header.height) {
        unsigned char len = 0;
        std::uint32_t color = 0;
        in.read((char*)&len, 1);
        unsigned char type = len >> 7;
        len &= 0x7f;
        if (type == 1) {
            in.read((char*)&color, 3);
            for (int i = 0; i <= len; i++) {
                data.push_back(color);
            } 
        } else {
            for (int i = 0; i <= len; i++) {
                in.read((char*)&color, 3);
                data.push_back(color);
            } 
        }

    }
} 

