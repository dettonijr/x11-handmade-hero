#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>

void write_color_to_pixmap(Framebuffer& f, int width, int height, unsigned char offset) {
    uint32_t* bitmap = f.get_raw_buffer();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t green = i << 2; 
            uint8_t blue = (j+offset) << 2;
            bitmap[i*width+j] = green << 8 | blue;
        }
    }
}
    
int main() {
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
        write_color_to_pixmap(frame, frame.width(), frame.height(), off++);
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
