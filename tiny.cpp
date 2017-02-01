#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "Obj.h"

int main() {
    Obj o("../african_head.obj");

    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 640, 480);
    d.map_window(w);      
    d.flush();;      

    printf("Depth %d\n", DefaultDepth(d.ptr(), DefaultScreen(d.ptr())));

    XSelectInput (d.ptr(), w.window,
                  ExposureMask |
                  StructureNotifyMask);

    Framebuffer& frame = w.get_framebuffer();

    XEvent ev;
    unsigned char off = 0;
    clock_t start = clock();
    for(int i = 0; ; i++) {
        while(XPending(d.ptr())) {
            XNextEvent(d.ptr(), &ev);
            if (ev.type == ConfigureNotify) {
                XConfigureEvent xce = ev.xconfigure;
                printf("Resizing %d %d\n", xce.width, xce.height);
                w.resize(xce.width, xce.height);
            }   
        }
        frame.fill(Color::White);
        frame.draw_line(13, 20, 80, 40, Color::Black);
        frame.draw_line(20, 13, 40, 80, Color::Red);
        frame.draw_line(80, 40, 13, 20, Color::Red);
        frame.draw_line(50, 50, 80, 75, Color::Blue);
        frame.draw_line(80, 75, 130, 75, Color::Blue);
        frame.draw_line(130, 75, 100, 50, Color::Blue);
        frame.draw_line(100, 50, 50, 50, Color::Blue);
        frame.draw_line(80, 75, 100, 50, Color::Blue);
        o.draw(frame);
        w.update();
        if (i % 100 == 0) {
            clock_t end = clock();
            unsigned long micros = end - start;
            start = clock();
            printf("FPS: %d\n", 100000000/micros);
        }
        usleep(3000);
    } 

    return(0);
}
