#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"
#include <string.h>

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

void Framebuffer::draw_rect(int minx, int miny, int maxx, int maxy, uint8_t r, uint8_t g, uint8_t b) {
    if (minx > _width || miny > _height)
        return;
    if (minx < 0)
        minx = 0;
    if (maxx > _width)
        maxx = _width;
    if (miny < 0)
        miny = 0;
    if (maxy > _height)
        maxy = _height;

    uint32_t color = r << 16 | g << 8 << b;

    for (int y = miny; y < maxy; y++) {
        uint32_t* pixelstart = buf + y*_width + minx;
        uint32_t* pixelend = pixelstart + maxx - minx;
        while (pixelstart < pixelend) {
             *pixelstart++ = color;
        }
    }
}
