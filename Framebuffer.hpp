#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <X11/Xlib.h>
#include <stdint.h>
#include "Color.h"
class X11Display;

class Framebuffer {
public:
    Framebuffer(X11Display& display, int width, int height);
    ~Framebuffer();

    void resize(int width, int height);
    uint32_t* get_raw_buffer();

    void set_pixel(int x, int y, Color& c);
    void fill(Color& c);
    void draw_line(int x0, int y0, int x1, int y1, Color& c);
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
