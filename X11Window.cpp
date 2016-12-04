#include "X11Window.hpp"
#include "X11Display.hpp"

X11Window::X11Window(X11Display* d, Window& w) {
    this->display = d;
    this->window = w;
    this->gc = XCreateGC(display->ptr(), window, 0, NULL);
}

X11Window::~X11Window() {
    XDestroyWindow(display->ptr(), window);
}
    
X11Window X11Window::CreateSimpleWindow(X11Display* d, int width, int height) {
    int screenId = DefaultScreen(d->ptr());
    Window w = XCreateSimpleWindow(d->ptr(), RootWindow(d->ptr(), screenId), 0, 0, width, height, 1, BlackPixel(d->ptr(), screenId), WhitePixel(d->ptr(), screenId));
    return X11Window(d, w);
}
    
void X11Window::draw(Framebuffer& f) {
    Display* dpy = display->ptr();
    XPutImage(dpy, window, gc, f.image(), 0, 0, 0, 0, f.width(), f.height());
    XSync(dpy, False);
}
