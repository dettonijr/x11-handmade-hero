#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>

void write_color_to_pixmap(uint32_t* bitmap, int width, int height, unsigned char offset) {
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

    Framebuffer frame(d, 640, 480);

    unsigned char off = 0;
    for(int i = 0; i < 1000; i++) {
        write_color_to_pixmap(frame.get_raw_buffer(), 640, 480, off++);
        w.draw(frame);
//        usleep(3000);
    } 

    return(0);
}
