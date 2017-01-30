#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int grid[14*22] = {
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

int getstate(int* grid, int i, int j) {
    if (i < 0 || i >= 14 || j < 0 || j >= 22)
        return 0;
    return *(grid+i*22+j);
}

void update_grid() {
    // Overshooting by 2 so we don't need to check boundaries
    int previous_row_buf[24] = {0};
    int* previous_row = &previous_row_buf[1];
    
    for (int i = 0; i < 14; i++) {
        int state_left = 0;
        int state_top_left = 0;
        for(int j = 0; j < 22; j++) {
            int state = getstate(grid, i, j);
            int sumaround = 0;
            sumaround += state_top_left;
            sumaround += previous_row[j];
            sumaround += previous_row[j+1];
            sumaround += state_left;
            sumaround += getstate(grid, i,   j+1);
            sumaround += getstate(grid, i+1, j-1);
            sumaround += getstate(grid, i+1, j);
            sumaround += getstate(grid, i+1, j+1);
            if (!state && sumaround == 3)
                *(grid+i*22+j) = 1;
            if (state && sumaround < 2)
                *(grid+i*22+j) = 0;
            if (state && sumaround > 3)
                *(grid+i*22+j) = 0;
            state_left = state;
            state_top_left = previous_row[j];
            previous_row[j] = state;
        }
    }
}

void draw_grid(Framebuffer& f) {
    int tilewidth = 30;
    int tileheight = 30;

    for(int i = 0; i < 14; i++) {
        for(int j = 0; j < 22; j++) {
            if(getstate(grid, i, j))
                f.draw_rect(j*tilewidth, i*tileheight, j*tilewidth+tilewidth, i*tileheight+tileheight, 0xff, 0x00, 0x00); 
            else
                f.draw_rect(j*tilewidth, i*tileheight, j*tilewidth+tilewidth, i*tileheight+tileheight, 0x00, 0x00, 0x00); 
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
    for(int i = 0; ; i++) {
        while(XPending(d.ptr())) {
            XNextEvent(d.ptr(), &ev);
            if (ev.type == ConfigureNotify) {
                XConfigureEvent xce = ev.xconfigure;
                printf("Resizing %d %d\n", xce.width, xce.height);
                w.resize(xce.width, xce.height);
            }   
        }
        update_grid();    
        draw_grid(frame);
        w.update();
        usleep(300000);
    } 

    return(0);
}
