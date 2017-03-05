#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <limits>
#include "Util.h"

#define I2F(x) ((float)x+0.5)
#define F2I(x) ((int)x)

Framebuffer::Framebuffer(X11Display& display, int width, int height) : _dpy(display.ptr()), _image(NULL)  {
    resize(width, height);
}

Framebuffer::~Framebuffer() {
    XFree(_image);
}

void Framebuffer::resize(int width, int height) {
    if (_width == width && _height == height) {
        printf("Same size. Do nothing\n");
        return;
    }

    buf.resize(width*height*2);
    zbuf.resize(width*height*2);
    _width = width;
    _height = height;
    if (_image != NULL) 
        XFree(_image);
    _image = XCreateImage(_dpy, DefaultVisual(_dpy, DefaultScreen(_dpy)), DefaultDepth(_dpy, DefaultScreen(_dpy)), ZPixmap, 0, (char*) &buf[0], _width, _height, 8, 0);
}

std::vector<uint32_t>& Framebuffer::get_raw_buffer() {
    return buf;
}

void Framebuffer::set_pixel(int x, int y, const Color& c) {
    if (x < 0)
        return;
    else if (x > _width)
        return;
    else if (y < 0)
        return;
    else if (y > _height)
        return;

    if (y*_width+x > (_width*_height << 2)) {
        printf("FUDEU\n");
    }

    buf[y*_width+x] = c.r << 16 | c.g << 8 | c.b;
}

void Framebuffer::fill(Color& c) {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            buf[i*_width+j] = c.r << 16 | c.g << 8 | c.b;
            zbuf[i*_width+j] = std::numeric_limits<float>::max();
        }
    }
}

void Framebuffer::draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, const Color& c) {
    int minx = _width-1;
    int maxx = 0;
    int miny = _height-1;
    int maxy = 0;

    minx = std::min(F2I(v0.x), minx);
    minx = std::min(F2I(v1.x), minx);
    minx = std::min(F2I(v2.x), minx);
    minx = std::max(0, minx);
    miny = std::min(F2I(v0.y), miny);
    miny = std::min(F2I(v1.y), miny);
    miny = std::min(F2I(v2.y), miny);
    miny = std::max(0, miny);
    maxx = std::max(F2I(v0.x), maxx);
    maxx = std::max(F2I(v1.x), maxx);
    maxx = std::max(F2I(v2.x), maxx);
    maxx = std::min(_width-1, maxx);
    maxy = std::max(F2I(v0.y), maxy);
    maxy = std::max(F2I(v1.y), maxy);
    maxy = std::max(F2I(v2.y), maxy);
    maxy = std::min(_height-1, maxy);

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
            float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
            float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
            bool inside = w0>=0 && w1>=0 && w2>=0;
            if (inside) {
                float z = v0.z * w0 + v1.z*w1 + v2.z*w2;
                if (z < zbuf[y*_width+x]) {
                    //printf("%d %d %d\n", x, y, z);
                    zbuf[y*_width+x] = z;
                    set_pixel(x,y,c);
                }
            }

        }
    }
}

void Framebuffer::draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, FragmentShader& shader) {
    int minx = _width-1;
    int maxx = 0;
    int miny = _height-1;
    int maxy = 0;

    minx = std::min(F2I(v0.x), minx);
    minx = std::min(F2I(v1.x), minx);
    minx = std::min(F2I(v2.x), minx);
    minx = std::max(0, minx);
    miny = std::min(F2I(v0.y), miny);
    miny = std::min(F2I(v1.y), miny);
    miny = std::min(F2I(v2.y), miny);
    miny = std::max(0, miny);
    maxx = std::max(F2I(v0.x), maxx);
    maxx = std::max(F2I(v1.x), maxx);
    maxx = std::max(F2I(v2.x), maxx);
    maxx = std::min(_width-1, maxx);
    maxy = std::max(F2I(v0.y), maxy);
    maxy = std::max(F2I(v1.y), maxy);
    maxy = std::max(F2I(v2.y), maxy);
    maxy = std::min(_height-1, maxy);

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
            float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
            float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
            bool inside = w0>=0 && w1>=0 && w2>=0;
            float area = edgeFunction(v0,v1,v2);
            w0 /= area;
            w1 /= area;
            w2 /= area;
            if (inside) {
                float z = v0.z * w0 + v1.z*w1 + v2.z*w2;
                if (z < zbuf[y*_width+x]) {
                    //printf("%d %d %d\n", x, y, z);
                    zbuf[y*_width+x] = z;
                    set_pixel(x,y,shader.fragment(x,y));
                }
            }

        }
    }

}

void Framebuffer::draw_line(const Point<float>& p0, const Point<float>& p1, const Color& c) {
    draw_line(Point<int>(p0.x,p0.y,p0.z), Point<int>(p1.x,p1.y,p1.z), c);
}

   
void Framebuffer::draw_line(const Point<int>& p0, const Point<int>& p1, const Color& c) {
    draw_line(p0.x, p0.y, p1.x, p1.y, c);
}

void Framebuffer::draw_line(int x0, int y0, int x1, int y1, const Color& c) {
    // TODO cipping
    int dx = std::abs(x0 - x1); 
    int dy = std::abs(y0 - y1);
    if (dx > dy) {
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        float deltaerr = dy/(float)dx;
        float err = deltaerr - 0.5f;

        int y = y0;
        for (int x = x0; x <= x1; x++) {
            err += deltaerr;
            if (err >= 0.5f) {
                err -= 1.0f;
                y += (y0 > y1 ? -1 : 1);
            }
            set_pixel(x, y, c); 
        } 
    } else {
        if (y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        float deltaerr = dx/(float)dy;
        float err = deltaerr - 0.5f;

        int x = x0;
        for (int y = y0; y <= y1; y++) {
            err += deltaerr;
            if (err >= 0.5f) {
                err -= 1.0f;
                x += (x0 > x1 ? -1 : 1);
            }
            set_pixel(x, y, c); 
        } 
    }
}

void Framebuffer::draw_rect(int minx, int miny, int maxx, int maxy, Color& c) {
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

    uint32_t color = c.r << 16 | c.g << 8 << c.b;

    for (int y = miny; y < maxy; y++) {
        uint32_t* pixelstart = &buf[y*_width] + minx;
        uint32_t* pixelend = pixelstart + maxx - minx;
        while (pixelstart < pixelend) {
             *pixelstart++ = color;
        }
    }
}
