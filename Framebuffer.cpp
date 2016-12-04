#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"

Framebuffer::Framebuffer(X11Display& display, int width, int height) {
    buf = (uint32_t*) malloc(width*height<<2);
    _width = width;
    _height = height;
    _dpy = display.ptr();
    _image = XCreateImage(_dpy, DefaultVisual(_dpy, DefaultScreen(_dpy)), DefaultDepth(_dpy, DefaultScreen(_dpy)), ZPixmap, 0, (char*) buf, _width, _height, 8, 0);
}

Framebuffer::~Framebuffer() {
    free(buf);
    XFree(_image);
}

void Framebuffer::resize(int width, int height) {
    if (_width == width && _height == height) {
        printf("Same size. Do nothing\n");
        return;
    }

    buf = (uint32_t*) realloc(buf, width*height<<2);
    _width = width;
    _height = height;
    XFree(_image);
    _image = XCreateImage(_dpy, DefaultVisual(_dpy, DefaultScreen(_dpy)), DefaultDepth(_dpy, DefaultScreen(_dpy)), ZPixmap, 0, (char*) buf, _width, _height, 8, 0);
}

uint32_t* Framebuffer::get_raw_buffer() {
    return buf;
}
