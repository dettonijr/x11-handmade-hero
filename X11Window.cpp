#include "X11Window.hpp"
#include "X11Display.hpp"

X11Window::X11Window(X11Display* d, Window& w) {
    this->display = d;
    this->window = w;
}

X11Window::~X11Window() {
    XDestroyWindow(display->ptr(), window);
}
    
X11Window X11Window::CreateSimpleWindow(X11Display* d, int width, int height) {
    int screenId = DefaultScreen(d->ptr());
    Window w = XCreateSimpleWindow(d->ptr(), RootWindow(d->ptr(), screenId), 0, 0, width, height, 1, BlackPixel(d->ptr(), screenId), WhitePixel(d->ptr(), screenId));
    return X11Window(d, w);
}
