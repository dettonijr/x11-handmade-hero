#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <X11/Xlib.h>
#include <stdint.h>
#include "Color.h"
#include "Point.h"
class X11Display;

class Framebuffer {
public:
    Framebuffer(X11Display& display, int width, int height);
    ~Framebuffer();

    void resize(int width, int height);
    uint32_t* get_raw_buffer();

    void set_pixel(int x, int y, const Color& c);
    void fill(Color& c);

    void draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, const Color& c);
    void draw_triangle(const Point<int>& v0, const Point<int>& v1, const Point<int>& v2, const Color& c);

    void draw_line(const Point<float>& p0, const Point<float>& p1, const Color& c);
    void draw_line(const Point<int>& p0, const Point<int>& p1, const Color& c);
    void draw_line(int x0, int y0, int x1, int y1, const Color& c);
    void draw_rect(int minx, int miny, int maxx, int maxy, Color& c);

    int width() { return _width; }
    int height() { return _height; }
    XImage* image() { return _image; }

private:
    int _width;
    int _height;
    uint32_t* buf;
    XImage* _image;
    Display* _dpy;
};

#endif
