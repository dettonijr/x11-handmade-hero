#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "Obj.h"

void draw_obj(Framebuffer& f, Obj& o) {
    int width = f.width();
    int height = f.height();
    for (int i = 0; i < o.faces.size(); i++) {
        std::vector<int> face = o.faces[i];
        for (int j = 0; j < 3; j++) {
            Point v0 = o.verts[face[j]];
            Point v1 = o.verts[face[(j+1)%3]];
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            f.draw_line(x0, y0, x1, y1, 0x00, 0x00, 0x00);
        }
    }
}

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
        frame.fill(0xff, 0xff, 0xff);
        frame.draw_line(13, 20, 80, 40, 0x00, 0x00, 0x00);
        frame.draw_line(20, 13, 40, 80, 0xff, 0x00, 0x00);
        frame.draw_line(80, 40, 13, 20, 0xff, 0x00, 0x00);
        draw_obj(frame, o);
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
