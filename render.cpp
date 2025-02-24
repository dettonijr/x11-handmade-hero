#include <stdlib.h>
#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdio.h>
#include "Obj.h"
#include <cmath>
#include "Renderer.h"

const float  PI_F=3.14159265358979f;

int main() {
    //Obj o("/mnt/c/Users/sauron/Documents/untitled.obj");
    Point<float> p(1,1,1);
    Transform t = Transform::translate(1.,1.,0)*Transform::rotX(PI_F/2);
    auto p2 = p*t;

    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 640, 640);
    d.map_window(w);      
    d.flush();;      

    printf("Depth %d\n", DefaultDepth(d.ptr(), DefaultScreen(d.ptr())));

    XSelectInput (d.ptr(), w.window,
                  ExposureMask |
                  StructureNotifyMask);

    TGAFile tga("./body_diffuse.tga");
    Renderer r;
    Obj o("./body.obj", tga);
    o.transform(Transform::rotX(PI_F));
    r.add_obj(o);  
    // for (float f = -1.; f < 2; f += 0.5) {
    //     for (float g = -1.; g < 2; g += 0.5) {
    //         Obj o("./african_head.obj", tga);
    //         o.transform(Transform::translate(f, g, 0.0) * Transform::scale(0.3, 0.3, 0.3) * Transform::rotZ(PI_F)); 
    //         r.add_obj(o);
    //     }
    // }
    r.set_light(Point<float>(0,0,1));
    Framebuffer& frame = w.get_framebuffer();

    XEvent ev;
    Point<float> t0[3] = {Point<float> (10, 70, 10), Point<float> (50, 160, 10), Point<float> (70,80,10)};
    Point<float> t1[3] = {Point<float> (180, 50, 10), Point<float> (150, 1, 10), Point<float> (70,180,10)};
    Point<float> t2[3] = {Point<float> (180, 150, 10), Point<float> (120, 160, 10), Point<float> (130,180,10)};
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

        Point<float> t[3] = {Point<float> (0, 0, -100), Point<float> (0, 50, -100), Point<float> (50,0,100)};
        int off = 50.0*std::sin(i/30.);
        Point<float> tt[3] = {Point<float> (off, 0, 0), Point<float> (50+off, 50, 0), Point<float> (50+off,0,0)};
        //frame.draw_triangle(tt[0], tt[1], tt[2], Color::Blue);
        //frame.draw_triangle(t[0], t[1], t[2], Color::Red);
        //frame.draw_triangle(t0[0], t0[1], t0[2], Color::Red);
        //frame.draw_triangle(t1[0], t1[1], t1[2], Color::White);
        //frame.draw_triangle(t2[0], t2[1], t2[2], Color::Green);
        //frame.draw_line(13, 20, 80, 40, Color::Black);
        //frame.draw_line(20, 13, 40, 80, Color::Red);
        //frame.draw_line(80, 40, 13, 20, Color::Red);
        //frame.draw_line(50, 50, 80, 75, Color::Blue);
        //frame.draw_line(80, 75, 130, 75, Color::Blue);
        //frame.draw_line(130, 75, 100, 50, Color::Blue);
        //frame.draw_line(100, 50, 50, 50, Color::Blue);
        //frame.draw_line(80, 75, 100, 50, Color::Blue);
        Transform tr = Transform::scale(300,300,300) * Transform::translate(1.,1.,0) * Transform::rotY(i*0.05);
        r.render(frame, tr);
        w.update();
        if (i % 10 == 0) {
            clock_t end = clock();
            unsigned long micros = end - start;
            start = clock();
            printf("FPS: %d\n", (int)(10000000/micros));
        }
    } 

    return(0);
}
