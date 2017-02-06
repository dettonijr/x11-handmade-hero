#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "Obj.h"
#include <cmath>

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
    Point<int> t0[3] = {Point<int> (10, 70, 0), Point<int> (50, 160, 0), Point<int> (70,80,0)};
    Point<int> t1[3] = {Point<int> (180, 50, 0), Point<int> (150, 1, 0), Point<int> (70,180,0)};
    Point<int> t2[3] = {Point<int> (180, 150, 0), Point<int> (120, 160, 0), Point<int> (130,180,0)};
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
        frame.fill(Color::Black);

        frame.draw_triangle(t0[0], t0[1], t0[2], Color::Red);
        frame.draw_triangle(t1[0], t1[1], t1[2], Color::White);
        frame.draw_triangle(t2[0], t2[1], t2[2], Color::Green);
        //frame.draw_line(13, 20, 80, 40, Color::Black);
        //frame.draw_line(20, 13, 40, 80, Color::Red);
        //frame.draw_line(80, 40, 13, 20, Color::Red);
        //frame.draw_line(50, 50, 80, 75, Color::Blue);
        //frame.draw_line(80, 75, 130, 75, Color::Blue);
        //frame.draw_line(130, 75, 100, 50, Color::Blue);
        //frame.draw_line(100, 50, 50, 50, Color::Blue);
        //frame.draw_line(80, 75, 100, 50, Color::Blue);
        o.draw(frame, Point<float>(0, std::sin(i/10.), std::cos(i/10.)));
        w.update();
        if (i % 10 == 0) {
            clock_t end = clock();
            unsigned long micros = end - start;
            start = clock();
            printf("FPS: %d\n", (int)(10000000/micros));
        }
        usleep(3000);
    } 

    return(0);
}
