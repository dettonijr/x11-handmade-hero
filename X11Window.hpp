#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <X11/Xlib.h>
#include "Framebuffer.hpp"
class X11Display;

class X11Window {
friend class X11Display;

public:
    X11Window(X11Display* d, Window& w, int width, int height);
    ~X11Window();

    static X11Window CreateSimpleWindow(X11Display* d, int width, int height);

    Framebuffer& get_framebuffer() { return frame; }

    void update();

    GC gc;
    Window window;
    X11Display* display;
    Framebuffer frame;
};

#endif
