#include <exception>
#include <iostream>
#include <stdexcept>
#include "X11Display.hpp"

X11Display::X11Display(const std::string& name) {
    std::cout << "LOL" << std::endl;
    display = XOpenDisplay(name.c_str());
    std::cout << "LL" << std::endl;
    if (display == NULL) {
        throw std::runtime_error("Display not found");
    }
}

X11Display::~X11Display() {
    if (display != NULL) {
        XCloseDisplay(display);
    }
}
    
void X11Display::map_window(X11Window& w) {
    XMapWindow(display, w.window);
}

void X11Display::flush() {
    XFlush(display);
}
    
