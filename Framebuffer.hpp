#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <X11/Xlib.h>
#include <stdint.h>
class X11Display;

class Framebuffer {
public:
    Framebuffer(X11Display& display, int width, int height);
    ~Framebuffer();

    void resize(int width, int height);
    uint32_t* get_raw_buffer();

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
