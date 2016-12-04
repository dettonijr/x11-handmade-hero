#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

class Framebuffer {
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void resize(int width, int height);
    unsigned char* get_raw_buffer();

    int width() { return _width; }
    int height() { return _height; }

private:
    int _width;
    int _height;
    unsigned char* buf;
};

#endif
