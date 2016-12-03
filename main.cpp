#include "X11Display.hpp"
#include "X11Window.hpp"
#include <unistd.h>

int main() {
    X11Display d("");
    
    X11Window w = X11Window::CreateSimpleWindow(&d, 320, 240);
    d.map_window(w);      
    d.flush();;      
    sleep(10);
    return 0;
}
