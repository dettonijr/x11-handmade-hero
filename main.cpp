#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>

void write_color_to_pixmap(char* bitmap, int width, int height, unsigned char offset) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bitmap[i*width*4+j*4] = (j+offset)<<2;
            bitmap[i*width*4+j*4+1] = i<<2;
            bitmap[i*width*4+j*4+2] = 0;
        }
    }
}

int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 640, 480);
    d.map_window(w);      
    d.flush();;      

    Framebuffer frame(640, 480);

    unsigned char off = 0;
    for(;;) {
        write_color_to_pixmap((char*) frame.get_raw_buffer(), 640, 480, off++);
        w.draw(frame);
        usleep(3000);
    } 

    return(0);
}
