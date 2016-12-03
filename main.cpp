#include "X11Display.hpp"
#include <unistd.h>

void write_color_to_pixmap(char* bitmap, int width, int height, unsigned char offset) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bitmap[i*width*4+j*4] = j+offset;
            bitmap[i*width*4+j*4+1] = i;
            bitmap[i*width*4+j*4+2] = 0;
        }
    }
}

int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 320, 240);
    d.map_window(w);      
    d.flush();;      

    Display* display = d.ptr();

    GC gc = XCreateGC(display, w.window, 0, NULL);
    XSetForeground(display, gc, WhitePixel(display, 0));
    XSetBackground(display, gc, BlackPixel(display, 0));

    XWindowAttributes attr;
    XGetWindowAttributes(display, w.window, &attr);

    Display* dpy = display;

    char * icon_bitmap_bits = (char*)malloc(320*240*4);

    unsigned char off = 0;
    for(;;) {
        write_color_to_pixmap(icon_bitmap_bits, 320, 240, off++);
        XImage* image = XCreateImage(dpy, DefaultVisual(dpy, DefaultScreen(dpy)), DefaultDepth(dpy, DefaultScreen(dpy)), ZPixmap, 0, 
                        (char*) icon_bitmap_bits, 320, 240, 8, 0);

        XPutImage(dpy, w.window, gc, image, 0, 0, 0, 0, 320,320);
        XSync(display, False);
        XFree(image);
        usleep(1000);
    } 

    write_color_to_pixmap(icon_bitmap_bits, 320, 240, 30);
    XSync(display, False);


    sleep(2);

    return(0);
}
