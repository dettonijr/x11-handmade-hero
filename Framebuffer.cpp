#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"
#include <string.h>
#include <stdio.h>
#include <cstdlib>

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

void Framebuffer::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0)
        return;
    else if (x > _width)
        return;
    else if (y < 0)
        return;
    else if (y > _height)
        return;

    buf[y*_width+x] = r << 16 | g << 8 | b;
}

void Framebuffer::fill(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            buf[i*_width+j] = r << 16 | g << 8 | b;
        }
    }
}

void Framebuffer::draw_line(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    // TODO clipping
    int dx = std::abs(x0 - x1); 
    int dy = std::abs(y0 - y1);

    if (dx > dy) {
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x = x0; x <= x1; x++) {
            float t = (x-x0)/(float)(x1-x0);
            int y = y0*(1.-t) + y1*t; 
            set_pixel(x, y, r, g, b); 
        } 
    } else {
        if (y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int y = y0; y <= y1; y++) {
            float t = (y-y0)/(float)(y1-y0);
            int x = x0*(1.-t) + x1*t; 
            set_pixel(x, y, r, g, b); 
        } 
    }
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
