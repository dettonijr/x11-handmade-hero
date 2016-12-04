#include "Framebuffer.hpp"
#include <stdlib.h>

Framebuffer::Framebuffer(int width, int height) {
    buf = (unsigned char*) malloc(width*height<<2);
    _width = width;
    _height = height;
}

Framebuffer::~Framebuffer() {
    free(buf);
}

void Framebuffer::resize(int width, int height) {
    buf = (unsigned char*) realloc(buf, width*height<<2);
    _width = width;
    _height = height;
}

unsigned char* Framebuffer::get_raw_buffer() {
    return buf;
}
