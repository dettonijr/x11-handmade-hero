#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"

Framebuffer::Framebuffer(X11Display& display, int width, int height) {
    buf = (unsigned char*) malloc(width*height<<2);
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
    buf = (unsigned char*) realloc(buf, width*height<<2);
    _width = width;
    _height = height;
    XFree(_image);
    _image = XCreateImage(_dpy, DefaultVisual(_dpy, DefaultScreen(_dpy)), DefaultDepth(_dpy, DefaultScreen(_dpy)), ZPixmap, 0, (char*) buf, _width, _height, 8, 0);
}

unsigned char* Framebuffer::get_raw_buffer() {
    return buf;
}
