#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "TGAFile.hpp"
    
int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 1024, 1024);
    d.map_window(w);      
    d.flush();;      

    printf("Depth %d\n", DefaultDepth(d.ptr(), DefaultScreen(d.ptr())));

    XSelectInput (d.ptr(), w.window,
                  ExposureMask |
                  StructureNotifyMask);

    Framebuffer& frame = w.get_framebuffer();
    TGAFile f("../african_head_diffuse.tga");

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
        std::copy(f.data.begin(), f.data.end(), frame.get_raw_buffer().begin());
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
